GUI Library for GameGuru (old name is FPS Creator: Reloaded) game engine.

Basic conceptions:
- UI presented as "overlay"
- "overlay" content "blocks"
- "block" rendered from html file with specific attributes
- "overlay" can be "non-modal" (can be used for indication, e.g. - HUD) and "modal" (can be used for input)

Library export some functions at Lua:
- void fpscUiInitialize() - must be used to initialize library basic structures
- int fpscUiCreateLayer(string name, table variables) - load all "Files/uilayerbank/$name/*.html" files as blocks, return layer id
- bool fpscUiShowLayer(int id) - show "non-modal" layer
- bool fpscUiShowModalLayer(int id) - show "modal" layer
- bool fpscUiHideLayer(int id) - hide layer (can be used only to "non-modal" layers)
- string|number fpscUiGetVariable(int id, string variableName) - get $variableName variable from layer id
- void fpscUiSetVariable(int id, string|number value) - set $variableName variable at layer id

Block html file (e.g. "Files/uilayerbank/main/healthbar.html" for healthbar subblock of main layer) can use next attributes at BODY tag:
- data-x - set X position in pixels
- data-y - set Y position in pixels
- data-width - set width in pixels
- data-height - set height in pixels
- data-alignment-x - set X alignment (can be "left", "right", "center")
- data-alignment-y - set Y alignment (can be "top", "bottom", "center")
- data-cutby - set filename of image to crop block region

JavaScript work is next:
- during block initialization, 'filledBindings' function called (you'll need to implement id)
- when LUA code or other block change variable value - 'updateBinding(variableName)' is called
- to get variable name you should use variables.get(vriableName)
- to set variable name you should use variables.get(vriableName, value)
- to close modal layer you show set modalOpened variable to 0 ('variables.set("modalOpened", 0)')

Layer config

Must be place in Files/uilayerbank/$layerName/config.ini.
Now use only one optional parameter Layer.cursor - provides relative path to cursor pointer image.
E.g. if in uilayerbank/dialog/config.ini we have next code

[Layer]
cursor=../common/modal-cursor.png

it'll find cursor pointer image at uilayerbank/common/modal-cursor.png