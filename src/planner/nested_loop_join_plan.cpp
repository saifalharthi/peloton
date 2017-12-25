//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// nested_loop_join_plan.cpp
//
// Identification: /peloton/src/planner/nested_loop_join_plan.cpp
//
// Copyright (c) 2015-17, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <memory>
#include <vector>

#include "planner/nested_loop_join_plan.h"

namespace peloton {
namespace planner {

NestedLoopJoinPlan::NestedLoopJoinPlan(
    JoinType join_type,
    std::unique_ptr<const expression::AbstractExpression> &&predicate,
    std::unique_ptr<const ProjectInfo> &&proj_info,
    std::shared_ptr<const catalog::Schema> &proj_schema,
    const std::vector<oid_t> &join_column_ids_left,
    const std::vector<oid_t> &join_column_ids_right)
    : AbstractJoinPlan(join_type, std::move(predicate), std::move(proj_info),
                       proj_schema) {
  join_column_ids_left_ = join_column_ids_left;
  join_column_ids_right_ = join_column_ids_right;
}

void NestedLoopJoinPlan::HandleSubplanBinding(
    UNUSED_ATTRIBUTE bool from_left,
    UNUSED_ATTRIBUTE const BindingContext &ctx) {}

std::unique_ptr<AbstractPlan> NestedLoopJoinPlan::Copy() const {
  std::unique_ptr<const expression::AbstractExpression> predicate_copy(
      GetPredicate()->Copy());

  std::shared_ptr<const catalog::Schema> schema_copy(
      catalog::Schema::CopySchema(GetSchema()));

  NestedLoopJoinPlan *new_plan = new NestedLoopJoinPlan(
      GetJoinType(), std::move(predicate_copy), GetProjInfo()->Copy(),
      schema_copy, join_column_ids_left_, join_column_ids_right_);

  return std::unique_ptr<AbstractPlan>(new_plan);
}

}  // namespace planner
}  // namespace peloton
