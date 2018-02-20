/*
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Product name: redemption, a FLOSS RDP proxy
Copyright (C) Wallix 2017
Author(s): Jonathan Poelen
*/

#pragma once

#include <memory>
#include <chrono>
#include <string>

#include "transport/socket_transport.hpp"
#include "utils/executor.hpp"
#include "utils/sugar/unique_fd.hpp"

class mod_api;
class Callback;
namespace gdi
{
    class GraphicApi;
};

struct SessionReactor
{
    using PrefixArgs = jln::prefix_args<>;

    using BasicExecutor = jln::BasicExecutorImpl<PrefixArgs>;

//     template<class... Ts>
//     using TopExecutor = jln::TopExecutor2<PrefixArgs, Ts...>;

    enum class EventType : int8_t
    {
        Timeout,
        Callback,
        Mod,
    };

//     struct Context
//     {
//         SessionReactor& session_reactor;
//         SocketTransport socket_transport;
//         void* first_data;
//         jln::UniquePtr<BasicExecutor> executor;
//
//         template<class... Args>
//         jln::TopExecutorBuilder<PrefixArgs, Args...>
//         init_executor(Args&&... args)
//         {
//             auto* p = TopExecutor<Args...>::New(
//                 this->session_reactor.timers, static_cast<Args&&>(args)...);
//             this->first_data = &jln::detail::get<0>(p->ctx);
//             this->executor.reset(p);
//             return {*p};
//         }
//     };

//     Context& create_socket(
//         const char * name, unique_fd sck, const char* ip_address, int port,
//         std::chrono::milliseconds recv_timeout,
//         SocketTransport::Verbose verbose, std::string* error_message)
//     {
//         return *this->contexts.emplace_back(std::unique_ptr<Context>(new Context{
//             *this,
//             {name, std::move(sck), ip_address, port, recv_timeout, verbose, error_message},
//             {},
//             {}
//         }));
//     }

    template<class Base>
    struct Container
    {
        void attach(Base& elem)
        {
            assert(this->elements.end() == this->get_elem_iterator(elem));
            this->elements.emplace_back(&elem);
        }

        void detach(Base& elem)
        {
            auto it = this->get_elem_iterator(elem);
            if (it != this->elements.end()) {
                *it = std::move(this->elements.back());
                this->elements.pop_back();
            }
        }

//     private:
        auto get_elem_iterator(Base& elem)
        {
            return std::find(this->elements.begin(), this->elements.end(), &elem);
        }

        template<class... Args>
        void exec(Args&&... args)
        {
            this->exec_impl(
                [](auto&){ return std::true_type{}; },
                static_cast<Args&&>(args)...
            );
        }

        template<class Predicate, class... Args>
        void exec_impl(Predicate pred, Args&&... args)
        {
            for (std::size_t i = 0; i < this->elements.size(); ) {
                auto* elem = this->elements[i];
                if (pred(*elem)) {
                    switch (elem->exec_action(static_cast<Args&&>(args)...)) {
                        case jln::ExecutorResult::ExitSuccess:
                        case jln::ExecutorResult::ExitFailure:
                            assert(false && "Exit");
                        case jln::ExecutorResult::Terminate:
                            this->elements.erase(this->elements.begin() + i);
                            break;
                        case jln::ExecutorResult::Nothing:
                            break;
                        case jln::ExecutorResult::NeedMoreData:
                            assert(false && "NeedMoreData");
                        case jln::ExecutorResult::Ready:
                            ++i;
                            break;
                    }
                }
                else {
                    ++i;
                }
            }
        }

        std::vector<Base*> elements;
    };

    using BasicTimer = jln::BasicTimer<jln::prefix_args<>>;
    using BasicTimerPtr = jln::UniquePtr<BasicTimer>;

    struct TimerContainer : Container<BasicTimer>
    {
        void update_time(BasicTimer& timer, std::chrono::milliseconds ms)
        {
            assert(this->elements.end() != this->get_elem_iterator(timer));
            (void)timer;
            (void)ms;
        }

        timeval get_next_timeout() const noexcept
        {
            auto it = std::min_element(
                this->elements.begin(), this->elements.end(),
                [](auto& a, auto& b) { return a->time() < b->time(); });
            return it == this->elements.end() ? timeval{-1, -1} : (*it)->time();
        }

        template<class... Args>
        void exec(timeval const& end_tv, Args&&... args)
        {
            this->exec_impl(
                [&](BasicTimer const& timer){ return timer.time() <= end_tv; },
                static_cast<Args&&>(args)...
            );
        }

        void info(timeval const& end_tv) {
            for (auto& timer : this->elements) {
                auto const tv = timer->time();
                LOG(LOG_DEBUG, "%p: %ld %ld %ld",
                    static_cast<void*>(timer), tv.tv_sec, tv.tv_usec, difftimeval(tv, end_tv).count());
            }
        }

        template<class... Args>
        using Elem = jln::Timer<TimerContainer&, BasicTimer::prefix_args, Args...>;
    };

    template<class T>
    using OwnerPtr = std::unique_ptr<T, jln::DeleteSelf<typename T::base_type>>;

    template<class... Args>
    jln::TimerBuilder<OwnerPtr<TimerContainer::Elem<Args...>>>
    create_timer(Args&&... args)
    {
        using Timer = TimerContainer::Elem<Args...>;
        return {jln::new_event<Timer>(this->timer_events_, static_cast<Args&&>(args)...)};
    }

    using CallbackEvent = jln::ActionBase<jln::prefix_args<Callback&>>;
    using CallbackEventPtr = jln::UniquePtr<CallbackEvent>;

    template<class... Args>
    auto create_callback_event(Args&&... args)
    {
        using Event = jln::Action<decltype((this->front_events_)), jln::prefix_args<Callback&>, Args...>;
        return jln::new_event<Event>(this->front_events_, static_cast<Args&&>(args)...);
    }


    using GraphicEvent = jln::ActionBase<jln::prefix_args<gdi::GraphicApi&>>;
    using GraphicEventPtr = jln::UniquePtr<GraphicEvent>;

    struct GraphicContainer : Container<GraphicEvent>
    {
        template<class... Args>
        using Elem = jln::Action<GraphicContainer&, GraphicEvent::prefix_args, Args...>;
    };

    template<class... Args>
    jln::ActionBuilder<OwnerPtr<GraphicContainer::Elem<Args...>>>
    create_graphic_event(Args&&... args)
    {
        using Action = GraphicContainer::Elem<Args...>;
        return {jln::new_event<Action>(this->graphic_events_, static_cast<Args&&>(args)...)};
    }

    using BasicFd = jln::BasicExecutorImpl<PrefixArgs>;

    struct TopFd : BasicFd
    {
        void set_timeout(std::chrono::milliseconds ms) noexcept
        {
            this->timeout.set_time(ms);
        }

        void set_timeout(timeval const& tv) noexcept
        {
            this->timeout.set_time(tv);
        }

        using OnTimerPtrFunc = jln::MakeFuncPtr<TopFd&, PrefixArgs>;
        OnTimerPtrFunc on_timeout = jln::default_action_function();

        // TODO do no attached
        BasicTimer timeout;
        int fd;
    };

    template<class EventContainer, class PrefixArgs_, class... Ts>
    struct FdImpl : jln::BasicEvent<
        FdImpl<EventContainer, PrefixArgs_, Ts...>,
        EventContainer,
        TopFd,
        jln::Executor2FdContext,
        Ts...>
    {
        template<class... Args>
        FdImpl(int fd, Args&&... args)
        : FdImpl::basic_event(static_cast<Args&&>(args)...)
        {
            this->fd = fd;
        }

        template<class F>
        void set_on_exit(F) noexcept
        {
            this->on_exit = jln::wrap_fn<F, FdImpl, jln::Executor2FdContext>();
        }

        template<class F>
        void set_on_timeout(F) noexcept
        {
            this->on_timeout = jln::wrap_fn<F, FdImpl, jln::Executor2FdTimeoutContext>();
        }
    };

    template<class EventContainer, class PrefixArgs_, class... Args>
    using Fd = FdImpl<EventContainer, PrefixArgs_,
        typename jln::detail::decay_and_strip<Args>::type...>;

    using TopFdPtr = jln::UniquePtr<TopFd>;

    struct TopFdContainer : Container<TopFd>
    {
        template<class... Args>
        using Elem = Fd<TopFdContainer&, TopFd::prefix_args, Args...>;
    };

    template<class... Args>
    jln::TopFdBuilder<OwnerPtr<TopFdContainer::Elem<Args...>>>
    create_fd_event(int fd, Args&&... args)
    {
        using EventFd = TopFdContainer::Elem<Args...>;
        return {jln::new_event<EventFd>(fd, this->fd_events_, static_cast<Args&&>(args)...)};
    }


    //std::vector<std::unique_ptr<Context>> contexts;
    Container<CallbackEvent> front_events_;
    GraphicContainer graphic_events_;
    TimerContainer timer_events_;
    TopFdContainer fd_events_;

    std::vector<CallbackEvent*> front_events()
    {
        return this->front_events_.elements;
    }

    std::vector<GraphicEvent*> graphic_events()
    {
        return this->graphic_events_.elements;
    }

    void set_event_next(/*BackEvent_t*/int)
    {
        // assert(is not already set)
        // TODO unimplemented
    }
};
