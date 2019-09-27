// Copyright (c) 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SOURCE_FUZZ_TRANSFORMATION_REPLACE_BOOLEAN_CONSTANT_WITH_CONSTANT_BINARY_H_
#define SOURCE_FUZZ_TRANSFORMATION_REPLACE_BOOLEAN_CONSTANT_WITH_CONSTANT_BINARY_H_

#include "source/fuzz/fact_manager.h"
#include "source/fuzz/protobufs/spirvfuzz_protobufs.h"
#include "source/fuzz/transformation.h"
#include "source/opt/ir_context.h"

namespace spvtools {
namespace fuzz {

class TransformationReplaceBooleanConstantWithConstantBinary
    : public Transformation {
 public:
  explicit TransformationReplaceBooleanConstantWithConstantBinary(
      const protobufs::TransformationReplaceBooleanConstantWithConstantBinary&
          message);

  TransformationReplaceBooleanConstantWithConstantBinary(
      const protobufs::IdUseDescriptor& id_use_descriptor, uint32_t lhs_id,
      uint32_t rhs_id, SpvOp comparison_opcode,
      uint32_t fresh_id_for_binary_operation);

  // - |message_.fresh_id_for_binary_operation| must not already be used by the
  //   module.
  // - |message_.id_use_descriptor| must identify a use of a boolean constant c.
  // - |message_.lhs_id| and |message.rhs_id| must be the ids of constant
  //   instructions with the same type
  // - |message_.opcode| must be suitable for applying to |message.lhs_id| and
  //   |message_.rhs_id|, and the result must evaluate to the boolean constant
  //   c.
  bool IsApplicable(opt::IRContext* context,
                    const FactManager& fact_manager) const override;

  // A new instruction is added before the boolean constant usage that computes
  // the result of applying |message_.opcode| to |message_.lhs_id| and
  // |message_.rhs_id| is added, with result id
  // |message_.fresh_id_for_binary_operation|.  The boolean constant usage is
  // replaced with this result id.
  void Apply(opt::IRContext* context, FactManager* fact_manager) const override;

  // The same as Apply, except that the newly-added binary instruction is
  // returned.
  opt::Instruction* ApplyWithResult(opt::IRContext* context,
                                    FactManager* fact_manager) const;

  protobufs::Transformation ToMessage() const override;

 private:
  protobufs::TransformationReplaceBooleanConstantWithConstantBinary message_;
};

}  // namespace fuzz
}  // namespace spvtools

#endif  // SOURCE_FUZZ_TRANSFORMATION_REPLACE_BOOLEAN_CONSTANT_WITH_CONSTANT_BINARY_H_
