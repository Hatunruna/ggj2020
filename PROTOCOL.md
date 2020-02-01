# Protocol for PeM

- Start of the game
  1. server (each): PemServerInitRole                   x
  2. server (all): PemServerStartVoteForCaptain         x
  3. clients (each): PemClientVoteForCaptain            x
  4. server (all): PemServerChooseCaptain               x
- Game loop
  1. client (captain): PemClientStartMoveAndPlay
  2. server (all): PemServerStartMoveAndPlay
  3. clients (each): PemClientMoveAndPlay
  4. server (all): PemServerUpdateShip
  5. server (each): PemServerResolution

