#pragma once

constexpr auto NAME                   = "XCoin";
constexpr auto VERSION                = "1.0.0";

constexpr auto COIN_NAME              = "Coin";
constexpr auto COIN_SYMBOL            = "coin";
constexpr auto COIN_PRECISION         = 4;

constexpr auto MINE_BITS              = '0';
// constexpr auto MINE_MEMO              = "Reward for the mined block";
constexpr auto MINE_REWARD            = 10 * 10000; // amount
// constexpr auto MINE_INTERVAL          = 100; // blocks
// constexpr auto MINE_REWARD_CORRECT    = 2; // blocks
constexpr auto MINE_DIFFICULTY        = 4;

constexpr auto BLOCK_VERSION          = 1;
constexpr auto BLOCK_MAX_TIME         = 10; // seconds
constexpr auto BLOCK_MAX_SIZE         = 1000000; // bytes

// constexpr auto CHAIN_DEFAULT_PATH     = "xcoin/blocks";
// constexpr auto CHAIN_DEFAULT_MAX_SIZE = 1000;
