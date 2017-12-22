//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// network_manager.h
//
// Identification: src/include/network/network_manager.h
//
// Copyright (c) 2015-17, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>

#include <arpa/inet.h>
#include <netinet/tcp.h>

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <pthread.h>
#include <sys/file.h>

#include "common/exception.h"
#include "common/logger.h"
#include "common/container/lock_free_queue.h"
#include "notifiable_task.h"
#include "connection_dispatcher_task.h"
#include "protocol_handler.h"
#include "network_state.h"

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/crypto.h>

namespace peloton {
namespace network {


class PelotonServer {
public:

  PelotonServer();


  static SSLLevel ssl_level_;
  static pthread_mutex_t *ssl_mutex_buf_;
  PelotonServer &SetupServer();

  void ServerLoop();

  void Close();

  void SetPort(int new_port);

 public:
  static int recent_connfd;
  static SSL_CTX *ssl_context;
  static std::string private_key_file_;
  static std::string certificate_file_;
  static std::string root_cert_file_;

private:
  // For logging purposes
  // static void LogCallback(int severity, const char *msg);

  uint64_t port_;             // port number
  int listen_fd_ = -1;         // server socket fd that PelotonServer is listening on
  size_t max_connections_;    // maximum number of connections

  std::string private_key_file_;
  std::string certificate_file_;

  std::shared_ptr<ConnectionDispatcherTask> dispatcher_task;

  void SetIsClosed(bool is_closed) { this->is_closed_ = is_closed; }


  static int verify_callback(int ok, X509_STORE_CTX *store);

  static void SSLInit();

  static void SetSSLLevel(SSLLevel ssl_level) { ssl_level_ = ssl_level; }

  static SSLLevel GetSSLLevel() { return ssl_level_; }

  static void LoadSSLFileSettings();

  static void SSLLockingFunction(int mode, int n, const char* file, int line);

  static unsigned long SSLIdFunction(void);

  static int SSLMutexSetup(void);

  static int SSLMutexCleanup(void);

 private:

  template<typename... Ts> void try_do(int(*func)(Ts...), Ts... arg);

  // For testing purposes
  bool started;
};

}
}