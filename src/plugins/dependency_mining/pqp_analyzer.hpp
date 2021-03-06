#pragma once

#include "candidate_strategy/abstract_dependency_candidate_rule.hpp"
#include "expression/abstract_expression.hpp"
#include "operators/abstract_operator.hpp"
#include "util.hpp"

namespace opossum {

class PQPAnalyzer {
 protected:
  friend class DependencyMiningPlugin;
  explicit PQPAnalyzer(const std::shared_ptr<DependencyCandidateQueue>& queue);

  void add_rule(std::unique_ptr<AbstractDependencyCandidateRule> rule);

  void run();

 private:
  void _add_if_new(DependencyCandidate& candidate);
  const std::shared_ptr<DependencyCandidateQueue>& _queue;
  std::unordered_set<DependencyCandidate> _known_candidates;
  std::unordered_map<LQPNodeType, std::vector<std::unique_ptr<AbstractDependencyCandidateRule>>> _rules;
};

}  // namespace opossum
