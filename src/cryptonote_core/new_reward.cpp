#include "include_base_utils.h"
using namespace epee;

#include "cryptonote_basic_impl.h"
#include "string_tools.h"
#include "serialization/binary_utils.h"
#include "serialization/vector.h"
#include "cryptonote_format_utils.h"
#include "cryptonote_config.h"
#include "misc_language.h"
#include "common/base58.h"
#include "crypto/hash.h"
#include "common/int-util.h"
#include "common/dns_utils.h"

bool get_block_reward(size_t median_size, size_t current_block_size, uint64_t already_generated_coins, uint64_t &reward, uint8_t version) {
    const int target = DIFFICULTY_TARGET;
    const int target_minutes = target / 60;
    const int emission_speed_factor = EMISSION_SPEED_FACTOR_PER_MINUTE - (target_minutes-1);
    //9.000.000.000
    //90.000.000.000.000.000
    uint64_t base_reward = (MONEY_SUPPLY - already_generated_coins) >> emission_speed_factor;
    if (base_reward < FINAL_SUBSIDY*target_minutes)
    {
      base_reward = FINAL_SUBSIDY*target_minutes;
    }

    uint64_t full_reward_zone = get_min_block_size(version);

    //make it soft
    if (median_size < full_reward_zone) {
      median_size = full_reward_zone;
    }

    if (current_block_size <= median_size) {
      reward = base_reward;
      return true;
    }

    if(current_block_size > 2 * median_size) {
      MERROR("Block cumulative size is too big: " << current_block_size << ", expected less than " << 2 * median_size);
      return false;
    }
}