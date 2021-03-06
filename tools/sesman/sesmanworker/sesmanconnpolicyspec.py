#!/usr/bin/python -O
# -*- coding: utf-8 -*-

cp_spec = {
  'session': {
    u'inactivity_timeout': 'inactivity_timeout',
  },
  'session_log': {
    u'keyboard_input_masking_level': 'keyboard_input_masking_level',
  },
  'rdp': {
    u'enable_nla': 'enable_nla',
    u'enable_kerberos': 'enable_kerberos',
    u'server_redirection': 'server_redirection',
    u'load_balance_info': 'load_balance_info',
    u'use_client_provided_alternate_shell': 'use_client_provided_alternate_shell',
    u'use_client_provided_remoteapp': 'use_client_provided_remoteapp',
    u'use_native_remoteapp_capability': 'use_native_remoteapp_capability',
    u'rail_disconnect_message_delay': 'remote_programs_disconnect_message_delay',
    u'use_session_probe_to_launch_remote_program': 'use_session_probe_to_launch_remote_program',
  },
  'session_probe': {
    u'session_probe': 'enable_session_probe',
    u'session_probe_use_smart_launcher': 'use_smart_launcher',
    u'session_probe_enable_launch_mask': 'enable_launch_mask',
    u'enable_session_probe_launch_mask': 'enable_launch_mask', # Deprecated, for compatibility only.
    u'session_probe_on_launch_failure': 'on_launch_failure',
    u'session_probe_launch_timeout': 'launch_timeout',
    u'session_probe_launch_fallback_timeout': 'launch_fallback_timeout',
    u'session_probe_start_launch_timeout_timer_only_after_logon': 'start_launch_timeout_timer_only_after_logon',
    u'session_probe_keepalive_timeout': 'keepalive_timeout',
    u'session_probe_on_keepalive_timeout': 'on_keepalive_timeout',
    u'session_probe_end_disconnected_session': 'end_disconnected_session',
    u'session_probe_enable_log': 'enable_log',
    u'session_probe_enable_log_rotation': 'enable_log_rotation',
    u'session_probe_disconnected_application_limit': 'disconnected_application_limit',
    u'session_probe_disconnected_session_limit': 'disconnected_session_limit',
    u'session_probe_idle_session_limit': 'idle_session_limit',
    u'session_probe_smart_launcher_clipboard_initialization_delay': 'smart_launcher_clipboard_initialization_delay',
    u'session_probe_smart_launcher_start_delay': 'smart_launcher_start_delay',
    u'session_probe_smart_launcher_long_delay': 'smart_launcher_long_delay',
    u'session_probe_smart_launcher_short_delay': 'smart_launcher_short_delay',
    u'session_probe_enable_crash_dump': 'enable_crash_dump',
    u'session_probe_handle_usage_limit': 'handle_usage_limit',
    u'session_probe_memory_usage_limit': 'memory_usage_limit',
    u'session_probe_ignore_ui_less_processes_during_end_of_session_check': 'ignore_ui_less_processes_during_end_of_session_check',
    u'session_probe_childless_window_as_unidentified_input_field': 'childless_window_as_unidentified_input_field',
    u'session_probe_public_session': 'public_session',
    u'session_probe_outbound_connection_monitoring_rules': 'outbound_connection_monitoring_rules',
    u'session_probe_process_monitoring_rules': 'process_monitoring_rules',
    u'session_probe_extra_system_processes': 'extra_system_processes',
    u'session_probe_windows_of_these_applications_as_unidentified_input_field': 'windows_of_these_applications_as_unidentified_input_field',
  },
  'server_cert': {
    u'server_cert_store': 'server_cert_store',
    u'server_cert_check': 'server_cert_check',
    u'server_access_allowed_message': 'server_access_allowed_message',
    u'server_cert_create_message': 'server_cert_create_message',
    u'server_cert_success_message': 'server_cert_success_message',
    u'server_cert_failure_message': 'server_cert_failure_message',
  },
  'vnc': {
    u'server_is_apple': 'server_is_apple',
    u'server_unix_alt': 'server_unix_alt',
  },
}
