#include "dependency_mining_plugin.hpp"

#include "dependency_mining/validation_state.hpp"
#include "hyrise.hpp"
#include "utils/timer.hpp"

namespace opossum {

std::string DependencyMiningPlugin::description() const { return "Dependency Mining Plugin"; }

DependencyMiningPlugin::DependencyMiningPlugin()
    : _queue(std::make_shared<DependencyCandidateQueue>()), _pqp_analyzer(_queue) {}

void DependencyMiningPlugin::start() {
  Timer timer;
  std::cout << "====================================================\nStarting DependencyMiningPlugin\n";
  _pqp_analyzer.run();
  std::vector<std::thread> validator_threads;
  const bool is_limited = MAX_VALIDATION_CANDIDATES > -1 || MAX_VALIDATION_TIME > std::chrono::seconds{-1};
  const bool use_time = is_limited ? MAX_VALIDATION_CANDIDATES < 0 : false;
  const auto validation_state =
      std::make_shared<ValidationState>(!is_limited, MAX_VALIDATION_CANDIDATES, use_time, MAX_VALIDATION_TIME);
  for (size_t validator_id{0}; validator_id < NUM_VALIDATORS; ++validator_id) {
    validator_threads.emplace_back(
        [&](size_t i) {
          const auto validator = std::make_unique<DependencyValidator>(_queue, i, validation_state);
          validator->start();
        },
        validator_id);
  }
  for (auto& thread : validator_threads) thread.join();
  std::cout << "Clear Cache" << std::endl;
  Hyrise::get().default_pqp_cache->clear();
  Hyrise::get().default_lqp_cache->clear();
  std::cout << "DependencyMiningPlugin finished in " << timer.lap_formatted() << std::endl;
}

void DependencyMiningPlugin::stop() {}

EXPORT_PLUGIN(DependencyMiningPlugin)

}  // namespace opossum
