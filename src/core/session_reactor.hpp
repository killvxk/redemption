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
#include <new>

#include "utils/executor.hpp"
#include "utils/sugar/scope_exit.hpp"
#include "utils/sugar/unique_fd.hpp"
#include "utils/log.hpp"
#include "core/error.hpp"

#include <functional>
#include <memory>
#include <cassert>
#include <exception>
#include <chrono>
#include <utility>
#include "cxx/diagnostic.hpp"
#include "utils/executor.hpp"
#include "core/error.hpp"


#ifndef NDEBUG
# define REDEMPTION_DEBUG_ONLY(...) __VA_ARGS__
#else
# define REDEMPTION_DEBUG_ONLY(...)
#endif

template<class>
struct BasicFd;

class SessionReactor;

namespace jln2
{
    using Reactor = SessionReactor;

    template<class... Ts> class TopExecutor;
    template<class... Ts> class GroupExecutor;
    template<class Tuple, class... Ts> struct GroupExecutorWithValues;
    template<class T> class SharedData;
    class SharedPtr;
    template<class... Ts> class TopSharedPtr;

    enum class [[nodiscard]] R : char
    {
        Next,
        Terminate,
        ExitSuccess,
        ExitError,
        Exception,
        CreateGroup,
        NeedMoreData,
        SubstituteExit,
        SubstituteAction,
        SubstituteTimeout,
        Ready,
        CreateContinuation,
    };

    struct ExitR
    {
        enum Status : char {
            Error = char(R::ExitError),
            Success = char(R::Next),
            Exception = char(R::Exception),
            Terminate = char(R::Terminate),
        };

        R to_result() const noexcept
        {
            return static_cast<R>(this->status);
        }

        Status status;
        ::Error& error;
    };

    enum class ExitStatus : bool {
        Error,
        Success,
    };

    namespace detail
    {
        template<bool HasAct, bool HasExit, class Top, class Group>
        struct [[nodiscard]] GroupExecutorBuilderImpl
        {
            explicit GroupExecutorBuilderImpl(Top& top, std::unique_ptr<Group>&& g) noexcept;

#ifndef NDEBUG
            ~GroupExecutorBuilderImpl()
            {
                assert(!this->g);
            }
#endif

            template<class F>
            auto on_action(F&& f) &&;

            template<class F>
            auto on_exit(F&& f) &&;

            auto propagate_exit() &&;

        private:
            Top& top;
            std::unique_ptr<Group> g;
        };

        struct BuilderInit
        {
            enum E
            {
                None,
                Action = 1,
                Exit = 2,
                Timer = 4,
                Timeout = 8,
            };
        };

        template<BuilderInit::E Has, class InitCtx>
        struct [[nodiscard]] TopExecutorBuilderImpl
        {
            explicit TopExecutorBuilderImpl(InitCtx&& init_ctx) noexcept;

            template<class F>
            auto on_action(F&& f) &&;

            template<class F>
            auto on_exit(F&& f) &&;

            template<class F>
            auto on_timeout(F&& f) &&;

            auto set_timeout(std::chrono::milliseconds ms) &&;

            auto disable_timeout() &&;

            auto propagate_exit() &&;

        private:
            InitCtx init_ctx;
        };

    #ifdef IN_IDE_PARSER
        struct Func
        {
            template<class F>
            Func(F) {}
        };

        struct /*[[nodiscard]]*/ GroupExecutorBuilder_Concept
        {
            template<class... Ts>
            explicit GroupExecutorBuilder_Concept(Ts&&...) noexcept;

            GroupExecutorBuilder_Concept on_action(Func) { return *this; }
            GroupExecutorBuilder_Concept on_exit(Func) { return *this; }
            GroupExecutorBuilder_Concept propagate_exit();
        };

        struct /*[[nodiscard]]*/ TopExecutorBuilder_Concept
        {
            template<class... Ts>
            explicit TopExecutorBuilder_Concept(Ts&&...) noexcept;

            TopExecutorBuilder_Concept disable_timeout() { return *this; }
            TopExecutorBuilder_Concept set_timeout(std::chrono::milliseconds) { return *this; }
            TopExecutorBuilder_Concept on_timeout(Func) { return *this; }
            TopExecutorBuilder_Concept on_action(Func) { return *this; }
            TopExecutorBuilder_Concept on_exit(Func) { return *this; }
            TopExecutorBuilder_Concept propagate_exit();

            operator SharedPtr ();

            template<class... Ts> operator TopSharedPtr<Ts...> ();
        };

        template<class Top, class Group>
        using GroupExecutorBuilder = GroupExecutorBuilder_Concept;

        template<class InitCtx>
        using TopExecutorBuilder = TopExecutorBuilder_Concept;
    #else
        template<class Top, class Group>
        using GroupExecutorBuilder = GroupExecutorBuilderImpl<0, 0, Top, Group>;

        template<class InitCtx>
        using TopExecutorBuilder = TopExecutorBuilderImpl<BuilderInit::None, InitCtx>;
    #endif
    }

    using jln::detail::decay_and_strip_t;

    enum class NextMode { ChildToNext, CreateContinuation, };

    template<class... Ts>
    struct GroupContext
    {
        GroupContext(TopExecutor<Ts...>& top, GroupExecutor<Ts...>& current_group) noexcept
        : top(top)
        , current_group(current_group)
        {}

        template<class... Us>
    #ifdef IN_IDE_PARSER
        detail::GroupExecutorBuilder_Concept create_sub_executor(Us&&...);
    #else
        auto create_sub_executor(Us&&...);
    #endif

        R exception(Error const& e) noexcept;
        R ready() noexcept { return R::Ready; }
        R need_more_data() noexcept { return R::NeedMoreData; }
        R terminate() noexcept { return R::Terminate; }
        R next() noexcept { return R::Next; }
        R exit_on_error() noexcept { return R::ExitError; }
        R exit_on_success() noexcept { return R::ExitSuccess; }
        R exit(ExitStatus status) noexcept {
            return (status == ExitStatus::Success)
                ? this->exit_on_success()
                : this->exit_on_error();
        }

        GroupContext& enable_timeout(bool enable = true) noexcept;
        GroupContext& disable_timeout() noexcept
        {
            this->enable_timeout(false);
            return *this;
        }

        int get_fd() const noexcept;
        void set_fd(int fd) noexcept;

        Reactor& get_reactor() const noexcept;

        timeval get_current_time() const noexcept;

        // void set_timeout(std::chrono::milliseconds ms) noexcept
        // {
        //     this->basic_fd.set_timeout(ms);
        // }
        //
        // void restart_timeout()
        // {
        //     this->basic_fd.restart_timeout();
        // }
        //
        // SessionReactor& get_reactor() const noexcept
        // {
        //     return this->basic_fd.get_reactor();
        // }
        //
        // timeval get_current_time() const noexcept
        // {
        //     return this->get_reactor().get_current_time();
        // }

    protected:
        TopExecutor<Ts...>& top;
        GroupExecutor<Ts...>& current_group;
    };

    template<class Tuple, class... Ts>
    struct TopContext : GroupContext<Ts...>
    {
        template<class F>
        R replace_action(F&& f);

        template<class F>
        R replace_exit(F&& f);

        TopContext& enable_timeout(bool enable = true) noexcept
        {
            this->GroupContext<Ts...>::enable_timeout(enable);
            return *this;
        }

        TopContext& disable_timeout() noexcept
        {
            this->GroupContext<Ts...>::disable_timeout();
            return *this;
        }
    };

    template<class... Ts>
    struct ContextExit
    {
        TopExecutor<Ts...>& top;
        GroupExecutor<Ts...>& current_group;

        // R need_more_data() noexcept { return R::NeedMoreData; }
        // R terminate() noexcept { return R::Terminate; }
        // R next() noexcept { return R::Next; }
        // R exit_on_error() noexcept { return R::ExitError; }
        // R exit_on_success() noexcept { return R::ExitSuccess; }
        // R exit(ExitStatus status) noexcept {
        //     return (status == ExitStatus::Success)
        //         ? this->exit_on_success()
        //         : this->exit_on_error();
        // }

        // template<class F>
        // R replace_action(F&& f);
    };

    template<class... Ts>
    struct ContextTimer
    {
        R exception(Error const& e) noexcept;
        R ready() noexcept { return R::Ready; }
        R terminate() noexcept { return R::Terminate; }
        R exit_on_error() noexcept { return R::ExitError; }
        R exit_on_success() noexcept { return R::ExitSuccess; }
        R exit(ExitStatus status) noexcept {
            return (status == ExitStatus::Success)
                ? this->exit_on_success()
                : this->exit_on_error();
        }

        ContextTimer& disable_timeout() noexcept;

        int get_fd() const noexcept;
        void set_fd(int fd) noexcept;

        TopExecutor<Ts...>& top;
        GroupExecutor<Ts...>& current_group;
    };

    template<class Tuple, class... Ts>
    struct TopContextTimer : ContextTimer<Ts...>
    {
        template<class F>
        TopContextTimer& replace_action(F&& f);

        template<class F>
        TopContextTimer& replace_exit(F&& f);

        template<class F>
        R set_or_disable_timeout(std::chrono::milliseconds ms, F&& f);

        TopContextTimer& disable_timeout() noexcept
        {
            ContextTimer<Ts...>::disable_timeout();
            return *this;
        }
    };

    template<class... Ts>
    struct TimerData
    {
        bool is_enabled = true;
        timeval tv {};
        std::chrono::milliseconds delay = std::chrono::milliseconds(-1);
        std::function<R(ContextTimer<Ts...>, Ts...)> on_timeout;

        void initialize_delay(std::chrono::milliseconds ms)
        {
            // TODO initialize tv
            this->delay = ms;
        }
    };

    template<class... Ts>
    struct GroupExecutor
    {
        std::function<R(GroupContext<Ts...>, Ts...)> on_action;
        std::function<R(ContextExit<Ts...>, ExitR er, Ts...)> on_exit;
        NextMode next_mode = NextMode::ChildToNext;
        GroupExecutor* next;

        // TODO replace by deleter function
        virtual ~GroupExecutor() {}
    };

    template<class Tuple, class... Ts>
    // TODO specialization for GroupExecutorWithValues<tuple<>, Ts...>
    struct GroupExecutorWithValues : GroupExecutor<Ts...>
    {
        using Base = GroupExecutor<Ts...>;

        REDEMPTION_DIAGNOSTIC_PUSH
        REDEMPTION_DIAGNOSTIC_CLANG_IGNORE("-Wmissing-braces")
        template<class... Us>
        GroupExecutorWithValues(Us&&... xs)
        : t{static_cast<Us&&>(xs)...}
        {}
        REDEMPTION_DIAGNOSTIC_POP

        template<class F>
        void on_action(F&& f)
        {
            Base::on_action = [f, this](GroupContext<Ts...> ctx, Ts... xs) mutable /*-> R*/ {
                return this->t.invoke(
                    f, TopContext<Tuple, Ts...>{ctx}, static_cast<Ts&>(xs)...);
            };
        }

        template<class F>
        void on_exit(F&& f)
        {
            Base::on_exit = [f, this](ContextExit<Ts...> ctx, ExitR er, Ts... xs) mutable /*-> R*/ {
                return this->t.invoke(f, ctx, er, static_cast<Ts&>(xs)...);
            };
        }

        Tuple t;
    };

    template<class... Fs>
    auto sequencer(Fs&&... fs)
    {
        return [=, i = 0](auto ctx, auto&&... xs) mutable /*-> R*/ {
            // TODO optimise switch
            int nth = 0;
            R r = R::ExitError;
            ((nth++ == i ? (void)(
                r = fs(ctx, static_cast<decltype(xs)&&>(xs)...)
            ) : void()), ...);

            switch (r) {
                case R::Next:
                    return i < int(sizeof...(fs)-1) ? ((void)++i, R::Ready) : R::Next;
                case R::CreateGroup:
                    ++i;
                    return R::CreateContinuation;
                case R::SubstituteTimeout:
                case R::SubstituteAction:
                case R::SubstituteExit:
                    ++i;
                    return r;
                default:
                    return r;
            }
        };
    }

    template<class F>
    auto propagate_exit(F&& f) noexcept
    {
        return [=](auto /*ctx*/, ExitR er, auto&&... xs) mutable /*-> R*/ {
            f(static_cast<decltype(xs)&&>(xs)...);
            return er.to_result();
        };
    }

    inline auto propagate_exit() noexcept
    {
        return [](auto /*ctx*/, ExitR er, [[maybe_unused]] auto&&... xs) /*-> R*/ {
            return er.to_result();
        };
    }

    template<error_type id, int errnum = 0>
    inline auto exit_with_error()
    {
        return [](auto ctx, [[maybe_unused]] auto&&... xs) /*-> R*/ {
            return ctx.exception(Error{id, errnum});
        };
    }

    template<class... Ts>
    struct TopExecutor
    {
        TopExecutor(Reactor& reactor, int fd)
        : fd(fd)
        , reactor(reactor)
        {}

        ~TopExecutor()
        {
            assert(!this->exec_is_running);

            auto* p = this->group;
            while (p) {
                delete std::exchange(p, p->next);
            }
        }

        void set_fd(int fd) noexcept
        {
            assert(fd >= 0);
            this->fd = fd;
        }

        int get_fd() const noexcept
        {
            return this->fd;
        }

        Reactor& get_reactor() const noexcept
        {
            return this->reactor;
        }

        void set_timeout(std::chrono::milliseconds ms) noexcept;

        void update_next_time() noexcept;

        void disable_timeout() noexcept
        {
            this->enable_timeout(false);
        }

        void enable_timeout(bool enable = true) noexcept
        {
            this->timer_data.is_enabled = enable;
        }

        void set_delay(std::chrono::milliseconds ms) noexcept
        {
            this->timer_data.delay = ms;
        }

        void set_time(timeval const& tv) noexcept
        {
            this->timer_data.tv = tv;
        }

        void add_group(std::unique_ptr<GroupExecutor<Ts...>>&& group)
        {
            group->next = this->group;
            this->group = group.release();
        }

        void sub_group(std::unique_ptr<GroupExecutor<Ts...>>&& group)
        {
            this->loaded_group = std::move(group);
        }

        bool exec_timeout(Ts&... xs)
        {
            REDEMPTION_DEBUG_ONLY(this->exec_is_running = true;)

            R r;

            try {
                auto& on_timeout = this->timer_data.on_timeout;
                do {
                    switch ((r = on_timeout(ContextTimer<Ts...>{*this, *this->group}, xs...))) {
                        case R::Terminate:
                        case R::Exception:
                        case R::ExitError:
                        case R::ExitSuccess:
                        case R::Next:
                            r = this->_exec_exit(r, xs...);
                            break;
                        case R::NeedMoreData:
                        case R::Ready:
                            break;
                        case R::SubstituteTimeout:
                            on_timeout = std::move(this->on_timeout_switch);
                            r = R::Ready;
                            break;
                        case R::CreateContinuation:
                            REDEMPTION_UNREACHABLE();
                        case R::CreateGroup:
                            REDEMPTION_UNREACHABLE();
                        case R::SubstituteAction:
                            REDEMPTION_UNREACHABLE();
                        case R::SubstituteExit:
                            REDEMPTION_UNREACHABLE();
                    }
                } while(0);
            }
            catch (Error const& e) {
                this->error = e;
                r = this->_exec_exit(R::Exception, xs...);
            }

            REDEMPTION_DEBUG_ONLY(this->exec_is_running = false;)

            switch (r) {
                case R::Exception:
                    throw this->error;
                case R::ExitSuccess:
                case R::Terminate:
                case R::ExitError:
                case R::Next:
                    return false;
                case R::NeedMoreData:
                case R::Ready:
                    return true;
                case R::CreateGroup:
                    REDEMPTION_UNREACHABLE();
                case R::SubstituteTimeout:
                    REDEMPTION_UNREACHABLE();
                case R::SubstituteAction:
                    REDEMPTION_UNREACHABLE();
                case R::SubstituteExit:
                    REDEMPTION_UNREACHABLE();
                case R::CreateContinuation:
                    REDEMPTION_UNREACHABLE();
            }

            REDEMPTION_UNREACHABLE();
        }

        bool exec_action(Ts&... xs)
        {
            REDEMPTION_DEBUG_ONLY(this->exec_is_running = true;)

            R r;

            try {
                do {
                    switch ((r = this->_exec_action(xs...))) {
                        case R::ExitSuccess:
                        case R::Exception:
                        case R::Terminate:
                        case R::ExitError:
                        case R::NeedMoreData:
                            break;
                        case R::Ready:
                        case R::Next:
                            continue;
                        case R::CreateContinuation:
                            REDEMPTION_UNREACHABLE();
                        case R::CreateGroup:
                            REDEMPTION_UNREACHABLE();
                        case R::SubstituteTimeout:
                            REDEMPTION_UNREACHABLE();
                        case R::SubstituteAction:
                            REDEMPTION_UNREACHABLE();
                        case R::SubstituteExit:
                            REDEMPTION_UNREACHABLE();
                    }
                } while(0);
            }
            catch (Error const& e) {
                this->error = e;
                r = this->_exec_exit(R::Exception, xs...);
            }

            REDEMPTION_DEBUG_ONLY(this->exec_is_running = false;)

            switch (r) {
                case R::Exception:
                    throw this->error;
                case R::ExitSuccess:
                case R::Terminate:
                case R::ExitError:
                case R::Next:
                    return false;
                case R::NeedMoreData:
                case R::Ready:
                    return true;
                case R::CreateGroup:
                    REDEMPTION_UNREACHABLE();
                case R::SubstituteTimeout:
                    REDEMPTION_UNREACHABLE();
                case R::SubstituteAction:
                    REDEMPTION_UNREACHABLE();
                case R::SubstituteExit:
                    REDEMPTION_UNREACHABLE();
                case R::CreateContinuation:
                    REDEMPTION_UNREACHABLE();
            }

            REDEMPTION_UNREACHABLE();
        }

    private:
        R _exec_action(Ts&... xs)
        {
            R const r = this->group->on_action(GroupContext<Ts...>{*this, *this->group}, xs...);
            switch (r) {
                case R::Terminate:
                case R::Exception:
                case R::ExitError:
                case R::ExitSuccess:
                case R::Next:
                    return this->_exec_exit(r, xs...);
                case R::NeedMoreData:
                case R::Ready:
                    return r;
                case R::CreateContinuation:
                    this->loaded_group->next = this->group;
                    this->group = this->loaded_group.release();
                    this->group->next_mode = NextMode::CreateContinuation;
                    return R::Ready;
                case R::CreateGroup:
                    this->loaded_group->next = this->group;
                    this->group = this->loaded_group.release();
                    return R::Ready;
                case R::SubstituteExit:
                    REDEMPTION_UNREACHABLE();
                    // this->group->on_exit = std::move(this->loaded_group->on_exit);
                    // this->loaded_group.reset();
                    // return R::Ready;
                case R::SubstituteAction:
                    this->group->on_action = std::move(this->loaded_group->on_action);
                    this->loaded_group.reset();
                    return R::Ready;
                case R::SubstituteTimeout:
                    REDEMPTION_UNREACHABLE();
                    // return R::Ready;
            }
            REDEMPTION_UNREACHABLE();
        }

        R _exec_exit(R r, Ts&... xs)
        {
            do {
                R const re = this->group->on_exit(
                    ContextExit<Ts...>{*this, *this->group},
                    ExitR{static_cast<ExitR::Status>(r), this->error},
                    xs...);
                NextMode next_mode;
                switch (re) {
                    case R::ExitSuccess:
                    case R::Next:
                        next_mode = this->group->next_mode;
                        delete std::exchange(this->group, this->group->next);
                        switch (next_mode) {
                            case NextMode::ChildToNext:
                                r = re;
                                break;
                            case NextMode::CreateContinuation:
                                return this->group ? R::Ready : R::Terminate;
                        }
                        break;
                    case R::Exception:
                    case R::Terminate:
                    case R::ExitError:
                        delete std::exchange(this->group, this->group->next);
                        r = re;
                        break;
                    case R::Ready:
                    case R::NeedMoreData:
                        return re;
                    case R::CreateGroup:
                    case R::SubstituteExit:
                    case R::SubstituteAction:
                    case R::SubstituteTimeout:
                        return R::Ready;
                    case R::CreateContinuation:
                        REDEMPTION_UNREACHABLE();
                }
            } while (this->group);

            return r;
        }

        int fd;
        GroupExecutor<Ts...>* group = nullptr;
        std::unique_ptr<GroupExecutor<Ts...>> loaded_group;
        Reactor& reactor;

        REDEMPTION_DEBUG_ONLY(bool exec_is_running = false;)

    public: // TODO to private
        TimerData<Ts...> timer_data;

    // private:
        std::function<R(ContextTimer<Ts...>, Ts...)> on_timeout_switch;

    public:
        Error error = Error(NO_ERROR);
    };

    struct SharedDataBase
    {
        enum class FreeCat { Value, Self, };
        SharedPtr* shared_ptr;
        void (*deleter) (SharedDataBase*, FreeCat) noexcept;
        SharedDataBase* next;

        void free_value() noexcept
        {
            this->deleter(this, FreeCat::Value);
        }

        void delete_self() noexcept
        {
            this->deleter(this, FreeCat::Self);
        }

        bool has_value() const noexcept
        {
            return bool(this->shared_ptr);
        }

    protected:
        void release_shared_ptr() noexcept;
    };

    struct [[nodiscard]] SharedPtr
    {
        SharedPtr(SharedDataBase* p = nullptr) noexcept
          : p(p)
        {
            assert(!p || !p->shared_ptr);

            if (this->p) {
                this->p->shared_ptr = this;
            }
        }

        SharedPtr(SharedPtr const&) = delete;
        SharedPtr& operator=(SharedPtr const&) = delete;

        SharedPtr(SharedPtr&& other) noexcept
        : p(std::exchange(other.p, nullptr))
        {
            this->p->shared_ptr = this;
        }

        SharedPtr& operator=(SharedPtr&& other) noexcept
        {
            assert(other.p != this->p);
            this->reset();
            this->p = std::exchange(other.p, nullptr);
            this->p->shared_ptr = this;
            return *this;
        }

        ~SharedPtr()
        {
            this->reset();
        }

        explicit operator bool() const noexcept
        {
            return bool(this->p);
        }

        void reset() noexcept
        {
            if (this->p) {
                this->p->free_value();
                this->p = nullptr;
            }
        }

        void detach() noexcept
        {
            if (this->p) {
                this->p->shared_ptr = nullptr;
                this->p = nullptr;
            }
        }

    private:
        friend class SharedDataBase;

        SharedDataBase* release() noexcept
        {
            return std::exchange(this->p, nullptr);
        }

    protected:
        SharedDataBase* p;
    };

    inline void SharedDataBase::release_shared_ptr() noexcept
    {
        if (this->shared_ptr) {
            this->shared_ptr->release();
            this->shared_ptr = nullptr;
        }
    }

    template<class T>
    struct SharedData : SharedDataBase
    {
        template<class... Ts>
        SharedData(Ts&&... xs)
        : u(static_cast<Ts&&>(xs)...)
        {
            this->deleter = [](SharedDataBase* p, FreeCat cat) noexcept {
                auto* self = static_cast<SharedData*>(p);
                switch (cat) {
                    case FreeCat::Value:
                        REDEMPTION_DEBUG_ONLY(self->is_deleted = true;)
                        self->release_shared_ptr();
                        self->u.value.~T();
                        break;
                    case FreeCat::Self:
                        assert(self->is_deleted);
                        delete self;
                        break;
                }
            };
        }

        T* operator->() { return &this->u.value; }
        T& operator*() { return this->u.value; }

        T& value() { return this->u.value; }

    private:
        union U{
            T value;

            template<class... Ts>
            U(Ts&&... xs) : value(static_cast<Ts&&>(xs)...){}
            ~U() { /* removed by this->deleter */}
        } u;

        REDEMPTION_DEBUG_ONLY(bool is_deleted = false;)
    };

    struct SharedDataDeleter
    {
        void operator()(SharedDataBase* p) const noexcept
        {
            p->free_value();
            p->delete_self();
        }
    };

    template<class... Ts>
    class TopSharedPtr : public SharedPtr
    {
        using Top = TopExecutor<Ts...>;
        using Data = SharedData<Top>;

        struct PtrInterface : protected SharedPtr
        {
            friend class TopSharedPtr;

            void update_timer(std::chrono::milliseconds ms) noexcept
            {
                this->top().delay = ms;
            }

            void set_time(timeval const& tv) noexcept
            {
                this->top().set_time(tv);
            }

            void set_delay(std::chrono::milliseconds ms) noexcept
            {
                this->top().set_delay(ms);
            }

            void disable_timeout() noexcept
            {
                this->top().disable_timeout();
            }

            void enable_timeout(bool enable = true) noexcept
            {
                this->top().enable_timeout(enable);
            }

        private:
            Top& top() noexcept
            {
                return static_cast<Data*>(this->p)->value();
            }
        };

    public:
        TopSharedPtr(Data* p = nullptr) noexcept
        : SharedPtr(p)
        {}

        PtrInterface* operator->() noexcept
        {
            return static_cast<PtrInterface*>(static_cast<SharedPtr*>(this));
        }
    };


#ifdef IN_IDE_PARSER
# define REDEMPTION_JLN2_CONCEPT(C) C
#else
# define REDEMPTION_JLN2_CONCEPT(C) auto
#endif

    template<class... Ts>
    struct TopContainer
    {
        using Top = TopExecutor<Ts...>;
        using Group = GroupExecutor<Ts...>;
        using Data = SharedData<Top>;

    public:
        using Ptr = TopSharedPtr<Ts...>;

    private:
        template<class Tuple>
        struct InitContext
        {
            std::unique_ptr<GroupExecutorWithValues<Tuple, Ts...>> g;
            std::unique_ptr<Data, SharedDataDeleter> data_ptr;
            TopContainer& cont;

            GroupExecutorWithValues<Tuple, Ts...>& group() noexcept
            {
                return *this->g;
            }

            Top& top() noexcept
            {
                return this->data_ptr->value();
            }

            template<class F>
            void on_timeout(F&& f)
            {
                auto& g = *this->g.get();
                this->top().timer_data.on_timeout = [f, &g](
                    ContextTimer<Ts...> ctx, Ts... xs
                ) mutable /*-> R*/ {
                    return g.t.invoke(
                        f, TopContextTimer<Tuple, Ts...>{ctx}, static_cast<Ts&>(xs)...);
                };
            }

            TopSharedPtr<Ts...> terminate_init()
            {
                assert(this->data_ptr);
                this->top().add_group(std::move(this->g));
                SharedDataBase* data_ptr = this->data_ptr.release();
                data_ptr->next = std::exchange(this->cont.node_executors.next, data_ptr);
                data_ptr->shared_ptr = nullptr;
                return TopSharedPtr<Ts...>(static_cast<Data*>(data_ptr));
            }
        };

    public:
        template<class... Us>
        REDEMPTION_JLN2_CONCEPT(detail::TopExecutorBuilder_Concept)
        create_top_executor(Reactor& reactor, int fd, Us&&... xs)
        {
            using Tuple = jln::detail::tuple<decay_and_strip_t<Us>...>;
            using Group = GroupExecutorWithValues<Tuple, Ts...>;
            using InitCtx = InitContext<Tuple>;
            auto* data = new Data{reactor, fd};
            return detail::TopExecutorBuilder<InitCtx>{
                InitCtx{
                    std::make_unique<Group>(static_cast<Us&&>(xs)...),
                    std::unique_ptr<Data, SharedDataDeleter>(data),
                    *this
                },
            };
        }

        TopContainer() noexcept
        {
            this->node_executors.next = nullptr;
        }

        ~TopContainer()
        {
            this->clear();
        }

        void clear()
        {
            while (this->node_executors.next) {
                SharedDataBase* p = &this->node_executors;
                while (p->next) {
                    SharedDataBase*const node = p->next;
                    SharedDataBase*const next = p->next->next;
                    if (node->has_value()) {
                        // TODO static_cast<Data&>(*p)->external_exit();
                        node->free_value();
                        p = node;
                    }
                    else {
                        p->next->delete_self();
                        p->next = next;
                    }
                }
            }
        }

    private:
        template<class F>
        static auto apply_f(F& f, SharedDataBase* node)
        {
            auto* data = static_cast<Data*>(node);
            return f(data->value().get_fd(), data->value());
        }

    public:
        template<class F>
        void for_each(F&& f)
        {
            SharedDataBase* node = this->node_executors.next;
            for (; node; node = node->next) {
                if (node->shared_ptr) {
                    apply_f(f, node);
                }
            }
        }

        bool is_empty() const noexcept
        {
            return !bool(this->node_executors.next);
        }

        template<class Predicate>
        bool exec_action(Predicate&& predicate, Ts... xs)
        {
            return this->_exec(predicate, [&](Top& top) {
                return top.exec_action(static_cast<Ts&>(xs)...);
            });
        }

        bool exec_timeout(timeval const end_tv, Ts... xs)
        {
            auto predicate = [&](int /*fd*/, Top& top){
                return top.timer_data.is_enabled && top.timer_data.tv <= end_tv;
            };
            return this->_exec(predicate, [&](Top& top) {
                return top.exec_timeout(static_cast<Ts&>(xs)...);
            });
        }

        SharedDataBase node_executors;

    private:
        template<class Pred, class F>
        bool _exec(Pred& predicate, F f)
        {
            SharedDataBase* node = &this->node_executors;
            while (node->next) {
                auto* cur = node->next;
                if (cur->shared_ptr) {
                    Top& top = static_cast<Data&>(*cur).value();
                    if (!apply_f(predicate, cur)) {
                        node = node->next;
                    }
                    else if (f(top)) {
                        top.update_next_time();
                        node = node->next;
                    }
                    else {
                        node->next = cur->next;
                        cur->free_value();
                        cur->delete_self();
                    }
                }
                else {
                    node->next = cur->next;
                    cur->delete_self();
                }
            }

            return bool(this->node_executors.next);
        }
    };



    template<class... Ts>
    ContextTimer<Ts...>& ContextTimer<Ts...>::disable_timeout() noexcept
    {
        this->top.disable_timeout();
        return *this;
    }

    template<class... Ts>
    int ContextTimer<Ts...>::get_fd() const noexcept
    {
        return this->top.get_fd();
    }

    template<class... Ts>
    void ContextTimer<Ts...>::set_fd(int fd) noexcept
    {
        this->top.set_fd(fd);
    }

    template<class Tuple, class... Ts>
    template<class F>
    TopContextTimer<Tuple, Ts...>& TopContextTimer<Tuple, Ts...>::replace_action(F&& f)
    {
        auto& group = static_cast<GroupExecutorWithValues<Tuple, Ts...>&>(this->current_group);
        group.on_action(static_cast<F&&>(f));
        return *this;
    }

    template<class Tuple, class... Ts>
    template<class F>
    TopContextTimer<Tuple, Ts...>& TopContextTimer<Tuple, Ts...>::replace_exit(F&& f)
    {
        auto& group = static_cast<GroupExecutorWithValues<Tuple, Ts...>&>(this->current_group);
        group.on_exit(static_cast<F&&>(f));
        return *this;
    }

    template<class Tuple, class... Ts>
    template<class F>
    R TopContextTimer<Tuple, Ts...>::set_or_disable_timeout(std::chrono::milliseconds ms, F&& f)
    {
        auto& group = static_cast<GroupExecutorWithValues<Tuple, Ts...>&>(this->current_group);
        // TODO same in TopContainer::InitCtx
        this->top.set_timeout(ms);
        if (ms.count()) {
            this->top.on_timeout_switch = [f, &group](ContextTimer<Ts...> ctx, Ts... xs) mutable -> R {
                return group.t.invoke(
                    f, TopContextTimer<Tuple, Ts...>{ctx}, static_cast<Ts&>(xs)...);
            };
            return R::SubstituteTimeout;
        }
        else {
            this->top.disable_timeout();
        }
        return R::Ready;
    }


    template<class... Ts>
    void TopExecutor<Ts...>::set_timeout(std::chrono::milliseconds ms) noexcept
    {
        this->timer_data.delay = ms;
        this->update_next_time();
    }

    template<class... Ts>
    void TopExecutor<Ts...>::update_next_time() noexcept
    {
        this->timer_data.tv = addusectimeval(
            this->timer_data.delay, this->reactor.get_current_time());
    }


    template<class... Ts>
    template<class... Us>
    REDEMPTION_JLN2_CONCEPT(detail::GroupExecutorBuilder_Concept)
    GroupContext<Ts...>::create_sub_executor(Us&&... xs)
    {
        using Top = TopExecutor<Ts...>;
        using Group = GroupExecutorWithValues<
            jln::detail::tuple<decay_and_strip_t<Us>...>, Ts...>;
        return detail::GroupExecutorBuilder<Top, Group>{
            this->top, std::make_unique<Group>(static_cast<Us&&>(xs)...)};
    }

    template<class... Ts>
    R GroupContext<Ts...>::exception(Error const& e) noexcept
    {
        this->top.error = e;
        return R::Exception;
    }

    template<class... Ts>
    int GroupContext<Ts...>::get_fd() const noexcept
    {
        return this->top.get_fd();
    }

    template<class... Ts>
    void GroupContext<Ts...>::set_fd(int fd) noexcept
    {
        return this->top.set_fd(fd);
    }

    template<class... Ts>
    GroupContext<Ts...>& GroupContext<Ts...>::enable_timeout(bool enable) noexcept
    {
        this->top.enable_timeout(enable);
        return *this;
    }

    template<class... Ts>
    Reactor& GroupContext<Ts...>::get_reactor() const noexcept
    {
        return this->top.get_reactor();
    }

    template<class... Ts>
    timeval GroupContext<Ts...>::get_current_time() const noexcept
    {
        return this->get_reactor().get_current_time();
    }


    template<class Tuple, class... Ts>
    template<class F>
    R TopContext<Tuple, Ts...>::replace_action(F&& f)
    {
        auto& group = static_cast<GroupExecutorWithValues<Tuple, Ts...>&>(this->current_group);
        // TODO inefficient
        auto g = std::make_unique<GroupExecutor<Ts...>>();
        // TODO same in GroupExecutorWithValues
        g->on_action = [f, &group](GroupContext<Ts...> ctx, Ts... xs) mutable -> R {
            return group.t.invoke(
                f, TopContext<Tuple, Ts...>{ctx}, static_cast<Ts&>(xs)...);
        };
        this->top.sub_group(std::move(g));
        return R::SubstituteAction;
    }

    template<class Tuple, class... Ts>
    template<class F>
    R TopContext<Tuple, Ts...>::replace_exit(F&& f)
    {
        auto& group = static_cast<GroupExecutorWithValues<Tuple, Ts...>&>(this->current_group);
        group.on_exit(static_cast<F&&>(f));
        return R::NeedMoreData;
    }


    template<class... Ts>
    R ContextTimer<Ts...>::exception(Error const& e) noexcept
    {
        this->top.error = e;
        return R::Exception;
    }


    template<bool HasAct, bool HasExit, class Top, class Group>
    detail::GroupExecutorBuilderImpl<HasAct, HasExit, Top, Group>::GroupExecutorBuilderImpl(
        Top& top, std::unique_ptr<Group>&& g) noexcept
    : top(top)
    , g(std::move(g))
    {}

    template<bool HasAct, bool HasExit, class Top, class Group>
    auto select_group_result(Top& top, std::unique_ptr<Group>&& g)
    {
        if constexpr (HasExit && HasAct) {
            top.sub_group(std::move(g));
            return R::CreateGroup;
        }
        else {
            return detail::GroupExecutorBuilderImpl<HasAct, HasExit, Top, Group>{top, std::move(g)};
        }
    }

    template<bool HasAct, bool HasExit, class Top, class Group>
    template<class F>
    auto detail::GroupExecutorBuilderImpl<HasAct, HasExit, Top, Group>::on_action(F&& f) &&
    {
        static_assert(!HasAct, "on_action is already used");
        this->g->on_action(static_cast<F&&>(f));
        return select_group_result<1, HasExit, Top, Group>(this->top, std::move(this->g));
    }

    template<bool HasAct, bool HasExit, class Top, class Group>
    template<class F>
    auto detail::GroupExecutorBuilderImpl<HasAct, HasExit, Top, Group>::on_exit(F&& f) &&
    {
        static_assert(!HasExit, "on_exit or propagate_exit is already used");
        this->g->on_exit(static_cast<F&&>(f));
        return select_group_result<HasAct, 1, Top, Group>(this->top, std::move(this->g));
    }

    template<bool HasAct, bool HasExit, class Top, class Group>
    auto detail::GroupExecutorBuilderImpl<HasAct, HasExit, Top, Group>::propagate_exit() &&
    {
        return this->on_exit([](auto /*ctx*/, ExitR er, [[maybe_unused]] auto&&... xs){
            return static_cast<R>(er.status);
        });
    }


    template<detail::BuilderInit::E Has, class InitCtx>
    detail::TopExecutorBuilderImpl<Has, InitCtx>::TopExecutorBuilderImpl(
        InitCtx&& init_ctx) noexcept
    : init_ctx(std::move(init_ctx))
    {}

    template<int Has, class InitCtx>
    auto select_top_result(InitCtx& init_ctx)
    {
        using E = detail::BuilderInit::E;
        if constexpr (Has == (E::Action | E::Exit | E::Timer | E::Timeout)) {
            return init_ctx.terminate_init();
        }
        else {
            return detail::TopExecutorBuilderImpl<E(Has), InitCtx>{std::move(init_ctx)};
        }
    }

    template<detail::BuilderInit::E Has, class InitCtx>
    template<class F>
    auto detail::TopExecutorBuilderImpl<Has, InitCtx>::on_action(F&& f) &&
    {
        static_assert(!(Has & detail::BuilderInit::Action), "on_action is already used");
        this->init_ctx.group().on_action(static_cast<F&&>(f));
        return select_top_result<Has | detail::BuilderInit::Action>(this->init_ctx);
    }

    template<detail::BuilderInit::E Has, class InitCtx>
    template<class F>
    auto detail::TopExecutorBuilderImpl<Has, InitCtx>::on_exit(F&& f) &&
    {
        static_assert(!(Has & detail::BuilderInit::Exit), "on_exit or propagate_exit is already used");
        this->init_ctx.group().on_exit(static_cast<F&&>(f));
        return select_top_result<Has | detail::BuilderInit::Exit>(this->init_ctx);
    }

    template<detail::BuilderInit::E Has, class InitCtx>
    auto detail::TopExecutorBuilderImpl<Has, InitCtx>::set_timeout(std::chrono::milliseconds ms) &&
    {
        static_assert(!(Has & detail::BuilderInit::Timeout), "set_timeout is already used");
        this->init_ctx.top().set_timeout(ms);
        return select_top_result<Has | detail::BuilderInit::Timeout>(this->init_ctx);
    }

    template<detail::BuilderInit::E Has, class InitCtx>
    auto detail::TopExecutorBuilderImpl<Has, InitCtx>::disable_timeout() &&
    {
        static_assert(!(Has & detail::BuilderInit::Timer), "disable_timeout or on_timeout are already used");
        this->init_ctx.top().disable_timeout();
        return select_top_result<
            Has | detail::BuilderInit::Timer | detail::BuilderInit::Timeout
        >(this->init_ctx);
    }

    template<detail::BuilderInit::E Has, class InitCtx>
    template<class F>
    auto detail::TopExecutorBuilderImpl<Has, InitCtx>::on_timeout(F&& f) &&
    {
        static_assert(!(Has & detail::BuilderInit::Timer), "disable_timeout or on_timeout are is already used");
        this->init_ctx.on_timeout(static_cast<F&&>(f));
        return select_top_result<Has | detail::BuilderInit::Timer>(this->init_ctx);
    }

    template<detail::BuilderInit::E Has, class InitCtx>
    auto detail::TopExecutorBuilderImpl<Has, InitCtx>::propagate_exit() &&
    {
        return this->on_exit([](auto /*ctx*/, ExitR er, [[maybe_unused]] auto&&... xs){
            return er.to_result();
        });
    }
}


template<class PrefixArgs_>
struct TopExecutorAdapter;

template<class... Ts>
struct TopExecutorAdapter<jln::prefix_args<Ts...>>
{
    using type = jln2::TopExecutor<Ts...>;
    using group = jln2::GroupExecutor<Ts...>;
};

template<class PrefixArgs_>
struct BasicFd : jln::BasicTimer<PrefixArgs_>
//, ExecutorBase<PrefixArgs_>
//, TopExecutorAdapter<PrefixArgs_>::type
{
    using prefix_args = PrefixArgs_;
    using base_type = BasicFd;

    typename TopExecutorAdapter<PrefixArgs_>::type top_;

    ///@{ BEGIN Top
    void add_group_executor(
        std::unique_ptr<typename TopExecutorAdapter<PrefixArgs_>::group>&& g)
    {
        this->top_.add_group_executor(std::move(g));
    }

    template<class F>
    void then(F&& f)
    {
        this->top_.then(static_cast<F&&>(f));
    }

    template<class F>
    void set_on_exit(F&& f)
    {
        this->top_.set_on_exit(static_cast<F&&>(f));
    }

    void set_propagate_exit() noexcept
    {
        this->top_.set_propagate_exit();
    }

    template<class... Us>
    bool exec(Us&&... xs)
    {
        return this->top_.exec(*this, static_cast<Us&&>(xs)...);
    }

    template<class... Us>
    bool exec_action(Us&&... xs) noexcept
    {
        return this->exec(static_cast<Us&&>(xs)...);
    }
    ///@} END Top

    // using jln::BasicExecutorImpl<PrefixArgs_>::delete_self;
    // using jln::BasicExecutorImpl<PrefixArgs_>::deleter;

    // using ExecutorBase<PrefixArgs_>::on_action;

    jln::BasicTimer<PrefixArgs_>& timer() noexcept { return *this; }

    BasicFd(int fd, SessionReactor& session_reactor) noexcept
    : fd(fd)
    , session_reactor(session_reactor)
    {}

    void set_fd(int fd) noexcept
    {
        assert(fd >= 0);
        LOG(LOG_DEBUG, "set_fd %d -> %d", this->fd, fd);
        this->fd = fd;
    }

    int get_fd() const noexcept
    {
        return this->fd;
    }

    void restart_timeout() noexcept;
    void set_timeout(std::chrono::milliseconds ms) noexcept;
    void disable_timeout() noexcept;

    SessionReactor& get_reactor() const noexcept
    {
        return this->session_reactor;
    }

private:
    int fd;
    SessionReactor& session_reactor;
    bool timer_is_disabled = false;
};

class mod_api;
class Callback;
class Inifile;
namespace gdi
{
    class GraphicApi;
}

enum BackEvent_t {
    BACK_EVENT_NONE = 0,
    BACK_EVENT_NEXT,
    BACK_EVENT_STOP = 4,
    BACK_EVENT_REFRESH,

    BACK_EVENT_RETRY_CURRENT,
};

struct SessionReactor
{
    using PrefixArgs = jln::prefix_args<>;

    enum class EventType : int8_t
    {
        Timeout,
        Callback,
        Mod,
    };

    enum class EventState : bool
    {
        Alive,
        Dead,
    };

    /*
     * Layout memory
     *
     *    +-------------------------------------------------------+
     *    |  Builder<Ptr>                                         |
     *    |   Ptr ptr =                                           |
     *    |     +-----------------------+---------------------+   |
     *    |     |  SharedPtrPrivate<T>  |  SharedPtrBase<T> p |   |
     *    |     +-----------------------+---------------------+   |
     *    |                                        ||             |
     *    +----------------------------------------||-------------+
     *                                             ||
     *                                            move
     *                                             ||
     *                                             \/
     *   .----> +-----------------------+------------------------+ <-- public
     *  /       |    SharedPtr<Base>    |  SharedPtrBase<Base> p |
     *  |       +-----------------------+------------------------+
     *  |    ______________________________________/
     *  |   /
     *  |   \__.+-----------------------+------------------------+
     *  |       |  SharedPtrBase<Base>  | SharedData<Base>* data |
     *  |       +-----------------------+------------------------+
     *  |       _________________________________/
     *  |      /
     *  |      \__.+--------------------+--------------+
     *  |          |   SharedDataBase   |   use_count  | <._
     *  +----------| - - - - - - - - - -|- -shared_ptr | <-- internal
     *             |                    |    deleter   | <-*
     *             +--------------------+-+----------+-|
     *             |  SharedData<Base>  | |   base   | | <-- public
     *             +--------------------+-|     ^    |-+
     *             |   SharedData<T>    | |   value  | | <-- private
     *             +--------------------+-+----------+-+
     */

    struct SharedDataBase
    {
        int use_count;
        void* shared_ptr = nullptr;
        void (*deleter) (SharedDataBase*) noexcept;

        void apply_deleter() noexcept
        {
            this->deleter(this);
        }

        bool alive() const noexcept
        {
            return bool(this->shared_ptr);
        }
    };

    template<class T>
    struct SharedData : SharedDataBase
    {
        T value;
    };

    template<class T>
    struct SharedPtrBase
    {
        SharedPtrBase() = default;
        SharedPtrBase(SharedPtrBase const&) = delete;
        SharedPtrBase& operator=(SharedPtrBase const&) = delete;

        template<class U>
        SharedPtrBase(SharedPtrBase<U>&& ptr) noexcept
          : data(reinterpret_cast<Data*>(std::exchange(ptr.data, nullptr)))
        {
            static_assert(std::is_base_of<T, U>::value);
        }

        template<class U>
        SharedPtrBase& operator=(SharedPtrBase<U>&& other) noexcept
        {
            static_assert(std::is_base_of<T, U>::value);
            this->data = reinterpret_cast<Data*>(std::exchange(other.data, nullptr));
            return *this;
        }

        explicit operator bool () const noexcept
        {
            return bool(this->data);
        }

        T* get() const noexcept
        {
            assert(this->data);
            return &this->data->value;
        }

        T* operator->() const noexcept
        {
            assert(this->data);
            return &this->data->value;
        }

        T& operator*() const noexcept
        {
            assert(this->data);
            return this->data->value;
        }

        void set_deleter(decltype(SharedDataBase::deleter) f) noexcept
        {
            assert(this->data);
            this->data->deleter = f;
        }

        void add_use() noexcept
        {
            assert(this->data);
            ++this->data->use_count;
        }

        using Data = SharedData<T>;

        SharedPtrBase(Data* ptr) noexcept
          : data(ptr)
        {}

        Data* data = nullptr;
    };

    struct SharedPtrAccess
    {
        template<class T>
        static auto& p(T& p_) noexcept
        {
            return p_.p;
        }
    };

    template<class T>
    class SharedPtr
    {
    protected:
        SharedPtrBase<T> p;
        friend SharedPtrAccess;

    public:
        using value_type = T;

        SharedPtr() = default;

        template<class U>
        SharedPtr(SharedPtr<U>&& other) noexcept
          : p(std::move(SharedPtrAccess::p(other)))
        {
            this->p.data->shared_ptr = &this->p;
        }

        template<class U>
        SharedPtr& operator=(SharedPtr<U>&& other) noexcept
        {
            assert(static_cast<SharedDataBase*>(SharedPtrAccess::p(other).data) != static_cast<SharedDataBase*>(this->p.data));
            this->reset();
            this->p = std::move(SharedPtrAccess::p(other));
            this->p.data->shared_ptr = &this->p;
            return *this;
        }

        ~SharedPtr()
        {
            this->reset();
        }

        explicit operator bool () const noexcept
        {
            return bool(this->p);
        }

        T* operator->() const noexcept
        {
            return this->p.operator->();
        }

        T& operator*() const noexcept
        {
            return *this->p;
        }

        void reset() noexcept
        {
            if (this->p) {
                this->p.data->apply_deleter();
            }
        }
    };

    template<class T>
    class SharedPtrPrivate : public SharedPtr<T>
    {
        using Data = typename SharedPtrBase<T>::Data;

        SharedPtrPrivate(Data* data) noexcept
        {
            this->p.data = data;
            this->p.data->shared_ptr = &this->p;
        }

        friend SharedPtrAccess;

    public:
        template<class F>
        void set_notify_delete(F f) noexcept
        {
            this->p.set_deleter(make_deleter(f));
        }

        template<class F>
        void initialize(F&& f)
        {
            this->p.data->ctx.invoke(static_cast<F&&>(f));
        }

        template<class F>
        static auto make_deleter(F = nullptr) noexcept
        {
            return [](SharedDataBase* base) noexcept -> void {
                LOG(LOG_DEBUG, "dealloc %p %s", static_cast<void*>(base), typeid(T).name());
                # ifndef NDEBUG
                base->deleter = [](SharedDataBase* p) noexcept {
                    LOG(LOG_DEBUG, "dealloc %p %s already delete", static_cast<void*>(p), typeid(T).name());
                    assert(!"already delete");
                };
                # endif

                static_cast<SharedPtrBase<T>*>(base->shared_ptr)->data = nullptr;
                base->shared_ptr = nullptr;
                --base->use_count;
                auto* data = static_cast<Data*>(base);
                assert(static_cast<void*>(data) == static_cast<void*>(base));
                if constexpr (!std::is_same<F, std::nullptr_t>::value) {
                    data->value.ctx.invoke(jln::make_lambda<F>());
                }
                data->value.~T();
            };
        }

        template<int Use = 1, class C, class... Args>
        static SharedPtrPrivate New(C&& c, Args&&... args)
        {
            Data* data = static_cast<Data*>(::operator new(sizeof(Data)));
            LOG(LOG_DEBUG, "new %p %s", static_cast<void*>(data), typeid(T).name());
            if constexpr (noexcept(T(static_cast<Args&&>(args)...))) {
                new(&data->value) T(static_cast<Args&&>(args)...);
            }
            else {
                bool failed = true;
                SCOPE_EXIT(if (failed) {
                    ::operator delete(data);
                });
                new(&data->value) T(static_cast<Args&&>(args)...);
                failed = false;
            }
            c.attach(data);
            data->shared_ptr = nullptr;
            data->use_count = Use + 1;
            data->deleter = SharedPtrPrivate::make_deleter(nullptr);
            return SharedPtrPrivate(data);
        }
    };


    template<class Builder>
    struct NotifyDeleterBuilderWrapper : Builder
    {
        using Builder::Builder;

        template<class NotifyDeleter>
        NotifyDeleterBuilderWrapper set_notify_delete(NotifyDeleter d) && noexcept
        {
            this->internal_value().set_notify_delete(d);
            return std::move(*this);
        }

        template<class F>
        NotifyDeleterBuilderWrapper initialize(F&& f) && noexcept
        {
            this->internal_value().initialize(static_cast<F&&>(f));
            return std::move(*this);
        }
    };

    template<class Base>
    struct Container
    {
        using Data = SharedData<Base>;

        template<class U>
        void attach(SharedData<U>* data_) noexcept
        {
            static_assert(std::is_base_of<Base, U>::value);
            auto* data = reinterpret_cast<Data*>(data_);
            assert(this->elements.end() == std::find(this->elements.begin(), this->elements.end(), data));
            this->elements.emplace_back(data);
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
            auto run_element = [&](auto& elem){
                if (!pred(elem.value)) {
                    return true;
                }
                switch (elem.value.exec_action(static_cast<Args&&>(args)...)) {
                    case jln::ExecutorResult::ExitSuccess:
                    case jln::ExecutorResult::ExitFailure:
                        assert(false && "Exit");
                        REDEMPTION_CXX_FALLTHROUGH;
                    case jln::ExecutorResult::Terminate:
                        assert(elem.alive());
                        LOG(LOG_DEBUG, "f = %p %d", static_cast<void*>(&elem), elem.use_count);
                        elem.apply_deleter();
                        return false;
                    case jln::ExecutorResult::NeedMoreData:
                        assert(false && "NeedMoreData");
                        REDEMPTION_CXX_FALLTHROUGH;
                    case jln::ExecutorResult::Nothing:
                    case jln::ExecutorResult::Ready:
                        return true;
                    case jln::ExecutorResult::CreateGroup:
                    case jln::ExecutorResult::Next:
                        ;
                }
                return true;
            };

            this->run_elements(run_element);
        }

        template<class RunElement>
        void run_elements(RunElement run_element)
        {
            auto& cont = this->elements;
            for (std::size_t i = 0; i < cont.size(); ++i) {
                auto* data_ptr = cont[i];
                if (!data_ptr->alive() || !run_element(*data_ptr)) {
                    auto alive_i = i;
                    if (--data_ptr->use_count == 0) {
                        LOG(LOG_DEBUG, "delete %p %s", static_cast<void*>(data_ptr), typeid(Base).name());
                        ::operator delete(data_ptr);
                    }
                    while (++i < cont.size()) {
                        auto* data_ptr = cont[i];
                        if (!data_ptr->alive() || run_element(*data_ptr)) {
                            cont[alive_i] = data_ptr;
                            ++alive_i;
                        }
                        else if (--data_ptr->use_count == 0) {
                            LOG(LOG_DEBUG, "delete %p %s", static_cast<void*>(data_ptr), typeid(Base).name());
                            ::operator delete(data_ptr);
                        }
                    }
                    cont.erase(cont.begin() + alive_i, cont.end());
                    break;
                }
            }
        }

        void clear()
        {
            for (auto* data_ptr : this->elements) {
                --data_ptr->use_count;
                LOG(LOG_DEBUG, "%p %d", static_cast<void*>(data_ptr), data_ptr->use_count);
                switch (data_ptr->use_count) {
                    case 2:
                        assert(data_ptr->alive());
                        break;
                    case 1:
                        if (data_ptr->alive()) {
                            data_ptr->apply_deleter();
                        }
                        break;
                }
            }
            for (auto* data_ptr : this->elements) {
                if (0 == data_ptr->use_count) {
                    LOG(LOG_DEBUG, "delete %p %s", static_cast<void*>(data_ptr), typeid(Base).name());
                    ::operator delete(data_ptr);
                }
            }
            this->elements.clear();
        }

        auto get_elem_iterator(Base& base)
        {
            return std::find_if(
                this->elements.begin(), this->elements.end(),
                [&](Data* data){ return &data->value == &base; });
        }

        // TODO CountedPointer
        std::vector<Data*> elements;

        template<template<class...> class Tpl, class... Args>
        using Elem = Tpl<typename Base::prefix_args, Args...>;

        template<template<class...> class Tpl, class... Args>
        using Ptr = SharedPtrPrivate<Elem<Tpl, Args...>>;
    };

    template<class Timer>
    struct BasicTimerContainer : Container<Timer>
    {
        void update_delay(Timer& timer, std::chrono::milliseconds ms)
        {
            assert(this->elements.end() != this->get_elem_iterator(timer));
            (void)timer;
            (void)ms;
        }

        void update_time(Timer& timer, timeval const& tv)
        {
            assert(this->elements.end() != this->get_elem_iterator(timer));
            (void)timer;
            (void)tv;
        }

        timeval get_next_timeout() const noexcept
        {
            auto it = std::min_element(
                this->elements.begin(), this->elements.end(),
                [](auto& a, auto& b) { return a->value.time() < b->value.time(); });
            return it == this->elements.end() ? timeval{-1, -1} : (*it)->value.time();
        }

        template<class... Args>
        // TODO end_tv = get_current_time()
        void exec(timeval const end_tv, Args&&... args)
        {
            this->exec_impl(
                [&](Timer const& timer){ return timer.time() <= end_tv; },
                static_cast<Args&&>(args)...
            );
        }

        void info(timeval const& end_tv) {
            for (auto& timer : this->elements) {
                auto const tv = timer->value.time();
                LOG(LOG_DEBUG, "%p: %ld %ld %ld, delay=%ld",
                    static_cast<void*>(timer), tv.tv_sec, tv.tv_usec, difftimeval(tv, end_tv).count(), timer->delay.count());
            }
        }

        template<class ReactorTimerWrapper, class... Args>
        using Elem = jln::Timer<ReactorTimerWrapper, typename Timer::prefix_args, Args...>;

        template<class ReactorTimerWrapper, class... Args>
        using Ptr = SharedPtrPrivate<Elem<ReactorTimerWrapper, Args...>>;

        template<class ReactorTimerWrapper, class... Args>
        NotifyDeleterBuilderWrapper<jln::TimerBuilder<Ptr<ReactorTimerWrapper, Args...>>>
        create_shared_ptr(ReactorTimerWrapper r, Args&&... args)
        {
            return Ptr<ReactorTimerWrapper, Args...>::New(*this, r, static_cast<Args&&>(args)...);
        }
    };


    struct ReactorWrapper
    {
        SessionReactor& get_reactor() const noexcept
        {
            return this->reactor;
        }

        SessionReactor& reactor;
    };

    template<class Base>
    struct ActionContainer : Container<Base>
    {
        template<class... Args>
        using Elem = jln::Action<ReactorWrapper, typename Base::prefix_args, Args...>;

        template<class... Args>
        using Ptr = SharedPtrPrivate<Elem<Args...>>;

        template<class... Args>
        using Builder = NotifyDeleterBuilderWrapper<jln::ActionBuilder<Ptr<Args...>>>;

        template<class... Args>
        Builder<Args...> create_shared_ptr(SessionReactor& r, Args&&... args)
        {
            return {Ptr<Args...>::New(*this, r, static_cast<Args&&>(args)...)};
        }
    };


    template<auto timers>
    struct ReactorTimerWrapper
    {
        SessionReactor& get_reactor() const noexcept
        {
            return this->reactor;
        }

        template<class Timer>
        void update_delay(Timer& timer, std::chrono::milliseconds ms)
        {
            (this->reactor.*timers).update_delay(timer, ms);
        }

        template<class Timer>
        void update_time(Timer& timer, timeval const& tv)
        {
            (this->reactor.*timers).update_time(timer, tv);
        }

        SessionReactor& reactor;
    };

    using BasicTimer = jln::BasicTimer<jln::prefix_args<>>;
    using BasicTimerPtr = SharedPtr<BasicTimer>;

    using TimerContainer = BasicTimerContainer<BasicTimer>;

    template<class... Args>
    auto create_timer(Args&&... args)
    {
        using W = ReactorTimerWrapper<&SessionReactor::timer_events_>;
        return this->timer_events_.create_shared_ptr(W{*this}, static_cast<Args&&>(args)...);
    }


    using GraphicTimer = jln::BasicTimer<jln::prefix_args<gdi::GraphicApi&>>;
    using GraphicTimerPtr = SharedPtr<GraphicTimer>;

    using GraphicTimerContainer = BasicTimerContainer<GraphicTimer>;

    template<class... Args>
    auto create_graphic_timer(Args&&... args)
    {
        using W = ReactorTimerWrapper<&SessionReactor::graphic_timer_events_>;
        return this->graphic_timer_events_.create_shared_ptr(W{*this}, static_cast<Args&&>(args)...);
    }


    using CallbackEvent = jln::ActionBase<jln::prefix_args<Callback&>>;
    using CallbackEventPtr = SharedPtr<CallbackEvent>;

    using CallbackContainer = ActionContainer<CallbackEvent>;

    template<class... Args>
    CallbackContainer::Builder<Args...>
    create_callback_event(Args&&... args)
    {
        return this->front_events_.create_shared_ptr(*this, static_cast<Args&&>(args)...);
    }


    using GraphicEvent = jln::ActionBase<jln::prefix_args<gdi::GraphicApi&>>;
    using GraphicEventPtr = SharedPtr<GraphicEvent>;

    using GraphicContainer = ActionContainer<GraphicEvent>;

    template<class... Args>
    GraphicContainer::Builder<Args...>
    create_graphic_event(Args&&... args)
    {
        return this->graphic_events_.create_shared_ptr(*this, static_cast<Args&&>(args)...);
    }


    using SesmanEvent = jln::ActionBase<jln::prefix_args<Inifile&>>;
    using SesmanEventPtr = SharedPtr<SesmanEvent>;

    using SesmanContainer = ActionContainer<SesmanEvent>;

    template<class... Args>
    SesmanContainer::Builder<Args...>
    create_sesman_event(Args&&... args)
    {
        return this->sesman_events_.create_shared_ptr(*this, static_cast<Args&&>(args)...);
    }


    using TopFdContainer = jln2::TopContainer<>;
    using TopFdPtr = TopFdContainer::Ptr;

    template<class... Args>
    REDEMPTION_JLN2_CONCEPT(jln2::detail::TopExecutorBuilder_Concept)
    create_fd_event(int fd, Args&&... args)
    {
        return this->fd_events_.create_top_executor(
            *this, fd, static_cast<Args&&>(args)...);
    }


    using GraphicFdContainer = jln2::TopContainer<gdi::GraphicApi&>;
    using GraphicFdPtr = GraphicFdContainer::Ptr;

    template<class... Args>
    REDEMPTION_JLN2_CONCEPT(jln2::detail::TopExecutorBuilder_Concept)
    create_graphic_fd_event(int fd, Args&&... args)
    {
        return this->graphic_fd_events_.create_top_executor(
            *this, fd, static_cast<Args&&>(args)...);
    }


    CallbackContainer front_events_;
    GraphicContainer graphic_events_;
    SesmanContainer sesman_events_;
    TimerContainer timer_events_;
    GraphicTimerContainer graphic_timer_events_;
    TopFdContainer fd_events_;
    GraphicFdContainer graphic_fd_events_;

    timeval current_time {};

    void set_current_time(timeval const& now)
    {
        assert(now >= this->current_time);
        this->current_time = now;
    }

    timeval get_current_time() const noexcept
    {
        //assert((this->current_time.tv_sec /*> -1*/) && "current_time is uninitialized. Used set_current_time");
        return this->current_time;
    }

    struct EnableGraphics
    {
        explicit EnableGraphics(bool enable) noexcept
          : enable(enable)
        {}

        explicit operator bool () const noexcept
        {
            return this->enable;
        }

        const bool enable;
    };

    timeval get_next_timeout(EnableGraphics enable_gd)
    {
        if ((enable_gd && this->graphic_events_.elements.size())
         || this->front_events_.elements.size()) {
            return {0, 0};
        }

        auto tv = this->timer_events_.get_next_timeout();
        auto update_tv = [&](timeval const& tv2){
            if (tv.tv_sec < 0) {
                tv = tv2;
            }
            else if (tv2.tv_sec >= 0) {
                tv = std::min(tv, tv2);
            }
        };
        auto top_update_tv = [&](int /*fd*/, auto& top){
            if (top.timer_data.is_enabled) {
                update_tv(top.timer_data.tv);
            }
        };

        this->fd_events_.for_each(top_update_tv);
        if (enable_gd) {
            update_tv(this->graphic_timer_events_.get_next_timeout());
            this->graphic_fd_events_.for_each(top_update_tv);
        }

        return tv;
    }

    template<class F>
    void for_each_fd(EnableGraphics enable_gd, F f)
    {
        auto g = [&f](int fd, auto& top){
            assert(fd != -1);
            f(fd, top);
        };
        this->fd_events_.for_each(g);
        if (enable_gd) {
            this->graphic_fd_events_.for_each(g);
        }
    }

    template<class GetGd>
    void execute_timers(EnableGraphics enable_gd, GetGd get_gd)
    {
        auto const end_tv = this->get_current_time();
        this->timer_events_.exec(end_tv);
        this->fd_events_.exec_timeout(end_tv);
        if (enable_gd) {
            this->graphic_timer_events_.exec(end_tv, get_gd());
            this->graphic_fd_events_.exec_timeout(end_tv, get_gd());
        }
    }

    template<class GetGd>
    void execute_timers_at(EnableGraphics enable_gd, timeval const& end_tv, GetGd get_gd)
    {
        this->set_current_time(end_tv);
        this->execute_timers(enable_gd, get_gd);
    }

    template<class IsSetElem>
    void execute_graphics(IsSetElem is_set, gdi::GraphicApi& gd)
    {
        this->graphic_events_.exec(gd);
        this->graphic_fd_events_.exec_action(is_set, gd);
    }

    template<class IsSetElem>
    void execute_events(IsSetElem is_set)
    {
        this->fd_events_.exec_action(is_set);
    }

    void execute_sesman(Inifile& ini)
    {
        this->sesman_events_.exec(ini);
    }

    void execute_callbacks(Callback& callback)
    {
        this->front_events_.exec(callback);
    }

    ~SessionReactor()
    {
        front_events_.clear();
        graphic_events_.clear();
        sesman_events_.clear();
        timer_events_.clear();
        graphic_timer_events_.clear();
        fd_events_.clear();
        graphic_fd_events_.clear();
    }

    auto& front_events()
    {
        return this->front_events_.elements;
    }

    auto& graphic_events()
    {
        return this->graphic_events_.elements;
    }

    bool has_graphics_event() const noexcept
    {
        return this->graphic_events_.elements.size() || !this->graphic_fd_events_.is_empty();
    }

    int signal = 0;
    void set_next_event(/*BackEvent_t*/int signal)
    {
        // LOG(LOG_DEBUG, "SessionReactor::set_next_event %d", signal);
        assert(!this->signal || this->signal == signal);
        this->signal = signal;
        // assert(is not already set)
        // TODO unimplemented
    }
    void set_event_next(/*BackEvent_t*/int signal)
    {
        this->set_next_event(signal);
    }
};


template<class PrefixArgs_>
void BasicFd<PrefixArgs_>::set_timeout(std::chrono::milliseconds ms) noexcept
{
    this->set_delay(ms);
    this->set_time(addusectimeval(this->delay, this->session_reactor.get_current_time()));
    if constexpr (std::is_same<jln::prefix_args<>, PrefixArgs_>::value) {
        this->session_reactor.timer_events_.update_delay(*this, ms);
    }
    else {
        this->session_reactor.graphic_timer_events_.update_delay(*this, ms);
    }
}

template<class PrefixArgs_>
void BasicFd<PrefixArgs_>::disable_timeout() noexcept
{
    if (this->timer_is_disabled) {
        return;
    }

    auto disable = [this](auto& cont){
        auto it = std::find_if(cont.begin(), cont.end(), [this](auto& p){
            return &p->value == this;
        });
        assert(it != cont.end());
        --(*it)->use_count;
        *it = std::move(cont.back());
        cont.pop_back();
    };

    if constexpr (std::is_same<jln::prefix_args<>, PrefixArgs_>::value) {
        disable(this->session_reactor.timer_events_.elements);
    }
    else {
        disable(this->session_reactor.graphic_timer_events_.elements);
    }

    this->timer_is_disabled = true;
}

template<class PrefixArgs_>
void BasicFd<PrefixArgs_>::restart_timeout() noexcept
{
    assert(this->delay.count() > 0);
    this->tv = addusectimeval(this->delay, this->session_reactor.get_current_time());
}
