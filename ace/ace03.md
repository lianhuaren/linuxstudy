#0  ACE_Dev_Poll_Reactor::work_pending_i (this=this@entry=0x6128a0, max_wait_time=max_wait_time@entry=0x0)
    at Dev_Poll_Reactor.cpp:960

#1  0x00007ffff7b284dc in ACE_Dev_Poll_Reactor::handle_events_i (this=this@entry=0x6128a0, 
    max_wait_time=max_wait_time@entry=0x0, guard=...) at Dev_Poll_Reactor.cpp:1033

#2  0x00007ffff7b28638 in ACE_Dev_Poll_Reactor::handle_events (this=0x6128a0, max_wait_time=0x0)
    at Dev_Poll_Reactor.cpp:1015

#3  0x00007ffff7b5dbcc in ACE_Reactor::run_reactor_event_loop (this=0x612e30, eh=0x0) at Reactor.cpp:220

#4  0x0000000000403640 in main () at Server.cpp:137

```cpp

int
ACE_Dev_Poll_Reactor::handle_events_i (ACE_Time_Value *max_wait_time,
                                       Token_Guard &guard)
{
  ACE_TRACE ("ACE_Dev_Poll_Reactor::handle_events_i");

  int result = 0;

  // Poll for events
  //
  // If the underlying event wait call was interrupted via the interrupt
  // signal (i.e. returned -1 with errno == EINTR) then the loop will
  // be restarted if so desired.
  do
    {
      result = this->work_pending_i (max_wait_time);
      if (result == -1 && (this->restart_ == 0 || errno != EINTR))
        ACE_ERROR ((LM_ERROR, ACE_TEXT("%t: %p\n"), ACE_TEXT("work_pending_i")));
    }
  while (result == -1 && this->restart_ != 0 && errno == EINTR);

  if (result == 0 || (result == -1 && errno == ETIME))
    return 0;
  else if (result == -1)
    {
      if (errno != EINTR)
        return -1;

      // Bail out -- we got here since the poll was interrupted.
      // If it was due to a signal registered through our ACE_Sig_Handler,
      // then it was dispatched, so we count it in the number of events
      // handled rather than cause an error return.
      if (ACE_Sig_Handler::sig_pending () != 0)
        {
          ACE_Sig_Handler::sig_pending (0);
          return 1;
        }
      return -1;
    }

  // Dispatch an event.
  return this->dispatch (guard);
}

int
ACE_Dev_Poll_Reactor::work_pending_i (ACE_Time_Value * max_wait_time)
{
  ACE_TRACE ("ACE_Dev_Poll_Reactor::work_pending_i");

  if (this->deactivated_)
    return 0;

#if defined (ACE_HAS_EVENT_POLL)
  if (this->event_.data.fd != ACE_INVALID_HANDLE)
#else
  if (this->start_pfds_ != this->end_pfds_)
#endif /* ACE_HAS_EVENT_POLL */
    return 1;  // We still have work_pending (). Do not poll for
               // additional events.

  ACE_Time_Value timer_buf (0);
  ACE_Time_Value *this_timeout =
    this->timer_queue_->calculate_timeout (max_wait_time, &timer_buf);

  // Check if we have timers to fire.
  int const timers_pending =
    ((this_timeout != 0 && max_wait_time == 0)
     || (this_timeout != 0 && max_wait_time != 0
         && *this_timeout != *max_wait_time) ? 1 : 0);

  long const timeout =
    (this_timeout == 0
     ? -1 /* Infinity */
     : static_cast<long> (this_timeout->msec ()));

#if defined (ACE_HAS_EVENT_POLL)

  // See if there are handlers that have to be resumed before waiting.
  {
    ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, grd, this->to_be_resumed_lock_, -1);
    this->epoll_wait_in_progress_ = true;
    for (Resume_Map::iterator i = this->to_be_resumed_.begin ();
         i != this->to_be_resumed_.end ();
         ++i)
      {
        // Make sure that 1) the handle is still registered,
        // 2) the registered handler is the one we're waiting to resume.
        Event_Tuple *info = this->handler_rep_.find (i->first);
        if (info != 0 && info->event_handler == i->second)
          {
            this->resume_handler_i (i->first);
          }
      }
    this->to_be_resumed_.clear ();
  }

  // Wait for an event.
  int const nfds = ::epoll_wait (this->poll_fd_,
                                 &this->event_,
                                 1,
                                 static_cast<int> (timeout));
  // Count on this being an atomic update; at worst, we may get an
  // extraneous notify() from dispatch_io_event.
  this->epoll_wait_in_progress_ = false;


```

(gdb) bt

#0  ClientService::open (this=0x614ff0, p=0x7fffffffe4a0) at Server.cpp:23

#1  0x0000000000405c19 in ACE_Acceptor<ClientService, ACE_SOCK_Acceptor>::activate_svc_handler (this=0x7fffffffe4a0, 
    svc_handler=0x614ff0) at /usr/local/include/ace/Acceptor.cpp:347

#2  0x0000000000405786 in ACE_Acceptor<ClientService, ACE_SOCK_Acceptor>::handle_input (this=0x7fffffffe4a0, listener=6)
    at /usr/local/include/ace/Acceptor.cpp:429

#3  0x00007ffff7b2779b in upcall (handle=6, callback=&virtual table offset 48, event_handler=0x7fffffffe4a0, 
    this=0x6128a0) at /root/mycode/ACE_wrappers/ace/Dev_Poll_Reactor.inl:86

#4  ACE_Dev_Poll_Reactor::dispatch_io_event (this=this@entry=0x6128a0, guard=...) at Dev_Poll_Reactor.cpp:1321

#5  0x00007ffff7b284b1 in ACE_Dev_Poll_Reactor::dispatch (this=this@entry=0x6128a0, guard=...)
    at Dev_Poll_Reactor.cpp:1082

#6  0x00007ffff7b285cb in ACE_Dev_Poll_Reactor::handle_events_i (this=this@entry=0x6128a0, 
    max_wait_time=max_wait_time@entry=0x0, guard=...) at Dev_Poll_Reactor.cpp:1059

#7  0x00007ffff7b28638 in ACE_Dev_Poll_Reactor::handle_events (this=0x6128a0, max_wait_time=0x0)
    at Dev_Poll_Reactor.cpp:1015
#8  0x00007ffff7b5dbcc in ACE_Reactor::run_reactor_event_loop (this=0x612e30, eh=0x0) at Reactor.cpp:220
#9  0x0000000000403640 in main () at Server.cpp:137

