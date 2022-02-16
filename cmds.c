#include <stdio.h>
#include "discord.h"
#include <inttypes.h>

void createSlashCommands(struct discord* client, u64_snowflake_t* appId, u64_snowflake_t* guildId) {
  u64_snowflake_t app_id = *appId;
  u64_snowflake_t guild_id = *guildId;

  if (guild_id) {
    // Register guild command
    struct discord_create_guild_application_command_params params = {
      .type = DISCORD_APPLICATION_COMMAND_CHAT_INPUT,
      .name = "info",
      .description = "Retrieve information about this bot.",
      .default_permission = true
    };

    discord_create_guild_application_command(client, app_id, guild_id, &params, NULL);
  } else {
    // Register gloval commands
    struct discord_create_global_application_command_params params = {
      .type = DISCORD_APPLICATION_COMMAND_CHAT_INPUT,
      .name = "info",
      .description = "Retrieve information about this bot.",
      .default_permission = true
    };

    discord_create_global_application_command(client, app_id, &params);
  }
}