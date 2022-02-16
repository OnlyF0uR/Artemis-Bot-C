#ifndef CMDS_DOT_H
#define CMDS_DOT_H
#include "discord.h"

void createSlashCommands(struct discord* client, u64_snowflake_t appId, u64_snowflake_t guildId);

#endif