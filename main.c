#include <stdio.h>
#include <stdlib.h>
#include "discord.h"
#include <inttypes.h>
#include <assert.h>
#include "cmds.h"
#include <unistd.h>

void on_ready(struct discord* client) {
  // Get the discord user (bot)
  const struct discord_user *bot = discord_get_self(client);

  // Print that the bot has logged in.
  log_info("Logged in as %s#%s!", bot->username, bot->discriminator);
}

void on_interaction_create(struct discord *client, const struct discord_interaction *interaction) {
  log_info("Interaction %" PRIu64 " received", interaction->id);

  struct discord_interaction_response params = {
    .type = DISCORD_INTERACTION_CALLBACK_CHANNEL_MESSAGE_WITH_SOURCE, // 4
    .data =
      &(struct discord_interaction_callback_data) {
        .embeds = (struct discord_embed *[]) {
          &(struct discord_embed) {
            .title = "Title",
            .description = "Desc",
          },
          NULL
        },
        .flags = DISCORD_INTERACTION_CALLBACK_DATA_EPHEMERAL // 1 << 6
      }
  };

  ORCAcode code;
  code = discord_create_interaction_response(
    client, interaction->id, interaction->token, &params, NULL);

  if (code) {
    log_error("%s", discord_strerror(code, client));
  }
}

static void print_usage(char *prog) {
  fprintf(stderr, "Usage: %s -a <AppID> -g [GuildId]\n", prog);
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  u64_snowflake_t g_app_id = 0;
  u64_snowflake_t g_guild_id = 0;
  
  int opt;
  while (-1 != (opt = getopt(argc, argv, "a:g:"))) {
    switch (opt) {
      case 'a': g_app_id = strtoull(optarg, NULL, 10); break;
      case 'g': g_guild_id = strtoull(optarg, NULL, 10); break;
      default: print_usage(argv[0]); break;
    }
  }

  if (!g_app_id) print_usage(argv[0]);

  // Define the discord client
  struct discord *client = discord_config_init("config.json");
  assert(client != NULL && "Could not initialize client");

  // Define the events
  discord_set_on_ready(client, &on_ready);
  discord_set_on_interaction_create(client, &on_interaction_create);

  createSlashCommands(client, &g_app_id, &g_guild_id);
  
  // Run the client
  discord_run(client);
  // Run a cleanup on the client
  discord_cleanup(client);

  return 0;
}
