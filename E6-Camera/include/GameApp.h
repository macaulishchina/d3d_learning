#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "DT.h"
#include "d3dApp.h"
#include "Resource.h"

#define CONST_BUFFER_NUMS 6
class GameApp : public D3DApp {
public:

    GameApp(HINSTANCE hInstance);
    ~GameApp() = default;

    bool Init();
    void OnResize();
    void UpdateScene(float dt);
    void DrawScene();

private:
    bool InitScene();
    void Update2DScene();

    ResourceController mController;

};


#endif