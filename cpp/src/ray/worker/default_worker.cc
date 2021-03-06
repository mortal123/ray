
#include <ray/api.h>
#include <ray/api/ray_config.h>
#include <ray/util/logging.h>

namespace ray {
namespace api {

int default_worker_main(int argc, char **argv) {
  RAY_LOG(INFO) << "CPP default worker started";
  RAY_CHECK(argc == 8);

  auto config = ray::api::RayConfig::GetInstance();
  config->run_mode = RunMode::CLUSTER;
  config->worker_type = ray::WorkerType::WORKER;
  config->store_socket = std::string(argv[1]);
  config->raylet_socket = std::string(argv[2]);
  config->node_manager_port = std::stoi(std::string(argv[3]));
  std::string redis_address = std::string(std::string(argv[4]));
  auto pos = redis_address.find(':');
  RAY_CHECK(pos != std::string::npos);
  config->redis_ip = redis_address.substr(0, pos);
  config->redis_port = std::stoi(redis_address.substr(pos + 1, redis_address.length()));
  config->redis_password = std::string(std::string(argv[5]));
  config->session_dir = std::string(std::string(argv[6]));

  Ray::Init();

  ::ray::CoreWorkerProcess::RunTaskExecutionLoop();
  return 0;
}

}  // namespace api
}  // namespace ray
