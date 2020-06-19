# Network Protocol

**Note:** All packets are sent using TCP only.

## Integer types

Integer types are represented with `s8`/`s16`/`s32`/`s64` and `u8`/`u16`/`u32`/`u64`.

The `s`/`u` means `signed`/`unsigned` and the number is the size of the type in bits.

## Login

### Clientbound

#### ClientOk

Packet sent from a server to accept a connection.

| Field name    | Field type | Notes                                                |
| ------------- | ---------- | ---------------------------------------------------- |
| Client ID     | u16        | ID assigned by the server for this client            |
| Singleplayer? | bool       | Whether or not the server runs a singleplayer game   |

#### ClientRefused

Packet sent from a server to refuse a connection.

_This packet has no field._

### Serverbound

#### ClientConnect

Packet sent from a client attempting to connect.

_This packet has no field._

#### ClientDisconnect

Packet sent from a client attempting to disconnect.

_This packet has no field._

## Game

### Clientbound

#### ServerClosed

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Message       | std::string | Why the server has been closed                       |

#### ChunkData

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Chunk X       | s32         | Chunk X coordinate                                   |
| Chunk Y       | s32         | Chunk Y coordinate                                   |
| Chunk Z       | s32         | Chunk Z coordinate                                   |
| Array         |             |                                                      |
| - Block data  | u32         | Block ID and data                                    |
| - Light value | u8          | Sunlight and torchlight values                       |

#### PlayerInvUpdate (clientbound)

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| ID            | u16         | Player ID                                            |
| Inventory     | Inventory   | Player inventory                                     |

#### PlayerPosUpdate (clientbound)

| Field name     | Field type  | Notes                                                |
| -------------- | ----------- | ---------------------------------------------------- |
| ID             | u16         | Player ID                                            |
| Player X       | double      | Player X coordinate                                  |
| Player Y       | double      | Player Y coordinate                                  |
| Player Z       | double      | Player Z coordinate                                  |
| Teleportation? | bool        | Whether or not this update is a teleportation        |

#### PlayerSpawn

| Field name     | Field type  | Notes                                                |
| -------------- | ----------- | ---------------------------------------------------- |
| ID             | u16         | Player ID                                            |
| Player X       | double      | Player X coordinate                                  |
| Player Y       | double      | Player Y coordinate                                  |
| Player Z       | double      | Player Z coordinate                                  |

#### PlayerChangeDimension

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| ID            | u16         | Player ID                                            |
| Spawn X       | s32         | Spawn X coordinate in the new dimension              |
| Spawn Y       | s32         | Spawn Y coordinate in the new dimension              |
| Spawn Z       | s32         | Spawn Z coordinate in the new dimension              |
| Dimension     | u16         | New dimension ID                                     |

#### BlockUpdate

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Block X       | s32         | Block X coordinate                                   |
| Block Y       | s32         | Block Y coordinate                                   |
| Block Z       | s32         | Block Z coordinate                                   |
| Block ID      | u32         | Block ID and data                                    |

#### BlockGUIData

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Width         | u16         | Width of the GUI                                     |
| Height        | u16         | Height of the GUI                                    |
| Centered?     | bool        | Whether or not the GUI is centered on the screen     |
| Widget defs   | Array       | List of widget definitions (see LuaGUI::show)        |

#### BlockInvUpdate (clientbound)

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Block X       | s32         | Block X coordinate                                   |
| Block Y       | s32         | Block Y coordinate                                   |
| Block Z       | s32         | Block Z coordinate                                   |
| Inventory     | Inventory   | Block inventory                                      |

#### BlockDataUpdate

| Field name     | Field type    | Notes                                                 |
| -------------- | ------------- | ----------------------------------------------------- |
| Block X        | s32           | Block X coordinate                                    |
| Block Y        | s32           | Block Y coordinate                                    |
| Block Z        | s32           | Block Z coordinate                                    |
| Metadata       | BlockMetadata | Block metadata                                        |
| Use alt tiles? | bool          | Whether or not the block should use alternative tiles |

#### RegistryData

| Field name     | Field type    | Notes                                                 |
| -------------- | ------------- | ----------------------------------------------------- |
| Registry       | Registry      | Registry                                              |

#### ChatMessage (clientbound)

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| ID            | u16         | Player ID (`0` means it's a server message)          |
| Message       | std::string | Message sent by the client                           |

#### EntitySpawn

| Field name    | Field type   | Notes                                                |
| ------------- | ------------ | ---------------------------------------------------- |
| ID            | entt::entity | Entity ID                                            |

#### EntityDespawn

| Field name    | Field type   | Notes                                                |
| ------------- | ------------ | ---------------------------------------------------- |
| ID            | entt::entity | Entity ID                                            |

#### EntityPosition

| Field name    | Field type   | Notes                                                |
| ------------- | ------------ | ---------------------------------------------------- |
| ID            | entt::entity | Entity ID                                            |
| Entity X      | double       | Entity X coordinate                                  |
| Entity Y      | double       | Entity Y coordinate                                  |
| Entity Z      | double       | Entity Z coordinate                                  |

#### EntityRotation

| Field name      | Field type   | Notes                                                |
| --------------- | ------------ | ---------------------------------------------------- |
| ID              | entt::entity | Entity ID                                            |
| Rotation        | glm::quat4   | Entity rotation quaternion                           |

#### EntityAnimation

| Field name    | Field type         | Notes                                                |
| ------------- | ------------------ | ---------------------------------------------------- |
| ID            | entt::entity       | Entity ID                                            |
| Animation     | AnimationComponent | Entity animation data                                |

#### EntityDrawableDef

| Field name    | Field type         | Notes                                                |
| ------------- | ------------------ | ---------------------------------------------------- |
| ID            | entt::entity       | Entity ID                                            |
| Drawable def  | DrawableDef        | Entity drawable component definition                 |

### Serverbound

#### ChunkRequest

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Chunk X       | s32         | Chunk X coordinate                                   |
| Chunk Y       | s32         | Chunk Y coordinate                                   |
| Chunk Z       | s32         | Chunk Z coordinate                                   |

#### PlayerPlaceBlock

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Block X       | s32         | Block X coordinate                                   |
| Block Y       | s32         | Block Y coordinate                                   |
| Block Z       | s32         | Block Z coordinate                                   |
| Block ID      | u32         | Block ID and data                                    |

#### PlayerDigBlock

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Block X       | s32         | Block X coordinate                                   |
| Block Y       | s32         | Block Y coordinate                                   |
| Block Z       | s32         | Block Z coordinate                                   |

#### PlayerInvUpdate (serverbound)

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Inventory     | Inventory   | Player inventory                                     |

#### PlayerPosUpdate (serverbound)

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Player X      | double      | Player X coordinate                                  |
| Player Y      | double      | Player Y coordinate                                  |
| Player Z      | double      | Player Z coordinate                                  |

#### PlayerRotUpdate

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Camera Yaw    | float       | Horizontal camera view angle                         |
| Camera Pitch  | float       | Vertical camera view angle                           |

#### PlayerHeldItemChanged

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Hotbar slot   | u8          | ID of the current hotbar slot                        |
| Item ID       | u16         | Current item ID (to check match with server)         |

#### BlockActivated

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Block X       | s32         | Block X coordinate                                   |
| Block Y       | s32         | Block Y coordinate                                   |
| Block Z       | s32         | Block Z coordinate                                   |
| Screen width  | u16         | Client screen width                                  |
| Screen height | u16         | Client screen height                                 |
| GUI scale     | u8          | Client GUI scale                                     |

#### BlockInvUpdate (serverbound)

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Block X       | s32         | Block X coordinate                                   |
| Block Y       | s32         | Block Y coordinate                                   |
| Block Z       | s32         | Block Z coordinate                                   |
| Inventory     | Inventory   | Block inventory                                      |

#### ChatMessage (serverbound)

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Message       | std::string | Message sent by the client                           |

#### KeyPressed

| Field name    | Field type  | Notes                                                |
| ------------- | ----------- | ---------------------------------------------------- |
| Key ID        | u16         | Key ID in the registry                               |
| Screen width  | u16         | Client screen width                                  |
| Screen height | u16         | Client screen height                                 |
| GUI scale     | u8          | Client GUI scale                                     |

