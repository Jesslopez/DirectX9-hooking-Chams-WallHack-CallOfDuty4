#include <windows.h>
#include <d3d9.h>
#include <iostream>
#include <d3dx9.h>
#include <vector>
#include <string>
using namespace std;

BYTE originalBytesDrawIndexPrimitive[5];
typedef HRESULT (APIENTRY* oDrawIndexedPrimitive)(
    LPDIRECT3DDEVICE9 pDevice,
    D3DPRIMITIVETYPE Type,
    INT BaseVertexIndex,
    UINT MinVertexIndex,
    UINT NumVertices,
    UINT StartIndex,
    UINT PrimitiveCount
);
oDrawIndexedPrimitive oDrawIndexedPrim = NULL;
IDirect3DPixelShader9 * shaderfront;
IDirect3DPixelShader9 * shaderback;

/*
* When we're trying to draw something with this
* number of vertices and primitive count we found that 
* we're trying to draw a marine, or part of it (such as 
* helment, hands, vest, etc...).
* These values have to be found manually by manually changing the
* amount of vertices and primitive counts while changing the 
* shader for whatever we're trying to draw until it lands on 
* the mesh of a marine and then we log these two values. 

*** Example code for a logger:

int np = 0;
int nv = 0;

if(NumVertices == nv || PrimitiveCount == np){

LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = NULL;
HRESULT hr = pDevice->GetStreamSource(0, &pVertexBuffer, &offset, &stride);  // Get the vertex buffer

    LPDIRECT3DTEXTURE9 pTexture = NULL;

        if(NumVertices == nv && PrimitiveCount == np){
            pDevice->SetPixelShader(shaderfront);
        }else{
            pDevice->SetPixelShader(shaderback);
        }


}

// Function to update nv and np values based on the current line index
void dispValues() {
        // Display the current values (only once per update)
        std::cout << "NumVertices: " << nv << ", PrimitiveCount: " << np << std::endl;
}

// Function to detect key presses
void DetectKeyPresses() {
    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        nv++;  
        dispValues();  
        Sleep(100); //small delay to avoid multiple key presses
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        nv--;
        dispValues();
        Sleep(100);
    }

    if (GetAsyncKeyState('I') & 0x8000) {
        np++;  
        dispValues();  
        Sleep(100);
    }

    if (GetAsyncKeyState('K') & 0x8000) {
        np--;  
        dispValues();  
        Sleep(100);
    }

    if (GetAsyncKeyState('E') & 0x8000) {
        np=0;  
        dispValues();  
        Sleep(100);
    }
    if (GetAsyncKeyState('W') & 0x8000) {
        nv=0;
        dispValues();
        Sleep(100);
    }

    if (GetAsyncKeyState('Z') & 0x8000) {
        nv+=100;  
        dispValues();  
        Sleep(100);
    }

    if (GetAsyncKeyState('X') & 0x8000) {
        np+=100; 
        dispValues();  
        Sleep(100);
    }
    

}
*/
#define Marines ((\
        ((NumVertices == 164 && PrimitiveCount == 172) || \
        (NumVertices == 183 && PrimitiveCount == 112) || \
        (NumVertices == 49 && PrimitiveCount == 50) || \
        (NumVertices == 275 && PrimitiveCount == 322) || \
        (NumVertices == 394 && PrimitiveCount == 274) || \
        (NumVertices == 599 && PrimitiveCount == 418) || \
        (NumVertices == 716 && PrimitiveCount == 962) || \
        (NumVertices == 855 && PrimitiveCount == 1100) || \
        (NumVertices == 976 && PrimitiveCount == 1372)) || \
        \
        /* Mid range values */ \
        ((NumVertices == 40 && PrimitiveCount == 40) || \
        (NumVertices == 127 && PrimitiveCount == 124) || \
        (NumVertices == 283 && PrimitiveCount == 170) || \
        (NumVertices == 296 && PrimitiveCount == 188) || \
        (NumVertices == 513 && PrimitiveCount == 674) || \
        (NumVertices == 596 && PrimitiveCount == 704) || \
        (NumVertices == 834 && PrimitiveCount == 770) || \
        (NumVertices == 1726 && PrimitiveCount == 1760) || \
        (NumVertices == 2301 && PrimitiveCount == 2364)) || \
        \
        /* M-F values */ \
        ((NumVertices == 628 && PrimitiveCount == 670) || \
        (NumVertices == 635 && PrimitiveCount == 654) || \
        (NumVertices == 1020 && PrimitiveCount == 872) || \
        (NumVertices == 1021 && PrimitiveCount == 922) || \
        (NumVertices == 1341 && PrimitiveCount == 1132) || \
        (NumVertices == 1572 && PrimitiveCount == 1166)) || \
        \
        /* Far values */ \
        ((NumVertices == 137 && PrimitiveCount == 100) || \
        (NumVertices == 193 && PrimitiveCount == 140) || \
        (NumVertices == 580 && PrimitiveCount == 380) || \
        (NumVertices == 587 && PrimitiveCount == 370) || \
        (NumVertices == 592 && PrimitiveCount == 374) || \
        (NumVertices == 634 && PrimitiveCount == 372)) \
))

HRESULT GenerateShader(IDirect3DDevice9 *pD3Ddev, IDirect3DPixelShader9 **pShader, float r, float g, float b)
{
	char szShader[256];
	ID3DXBuffer *pShaderBuf = NULL;
	sprintf(szShader, "ps.1.1\ndef c0, %f, %f, %f, %f\nmov r0,c0", r, g, b, 1.0f);
	D3DXAssembleShader(szShader, sizeof(szShader), NULL, NULL, 0, &pShaderBuf, NULL);
	if (FAILED(pD3Ddev->CreatePixelShader((const DWORD*)pShaderBuf->GetBufferPointer(), pShader)))return E_FAIL;
	return D3D_OK;
}

HRESULT APIENTRY hookedDrawIndexPrimitive(    
    LPDIRECT3DDEVICE9 pDevice,
    D3DPRIMITIVETYPE Type,
    INT BaseVertexIndex,
    UINT MinVertexIndex,
    UINT NumVertices,
    UINT StartIndex,
    UINT PrimitiveCount
){
    //Remove hook to call original function
    DWORD oldProtect;
    VirtualProtect(oDrawIndexedPrim,5,PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(oDrawIndexedPrim,originalBytesDrawIndexPrimitive,5);
    HRESULT result = 0;
    // VirtualProtect(oDrawIndexedPrim , 5, oldProtect, &oldProtect);

    if (Marines) {
        pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
        pDevice->SetPixelShader(shaderfront);
        result = oDrawIndexedPrim(pDevice, Type,BaseVertexIndex,MinVertexIndex,NumVertices,StartIndex,PrimitiveCount);
        pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    } else {
        result = oDrawIndexedPrim(pDevice, Type,BaseVertexIndex,MinVertexIndex,NumVertices,StartIndex,PrimitiveCount);
    }

    //recreate the hook
    //calc relative address since we're doing a near jmp
    DWORD hookFuncAddr = (DWORD)hookedDrawIndexPrimitive;
    DWORD relativeAddress = hookFuncAddr - (DWORD)oDrawIndexedPrim - 5;

    //create jump instruction
    BYTE insts[5] = {0xE9};
    memcpy(&insts[1],&relativeAddress,4);

    VirtualProtect(oDrawIndexedPrim,5,PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(oDrawIndexedPrim,&insts,5);

return result;
}

void hkDrawIndexPrimitive(LPDIRECT3DDEVICE9 device){
    //we get the vtable and the function addresss
    DWORD* vtable = *(DWORD**)device;
    oDrawIndexedPrim = (oDrawIndexedPrimitive)vtable[82];
    //save original bytes
    memcpy(originalBytesDrawIndexPrimitive,oDrawIndexedPrim,5);

    //calc relative address since we're doing a near jmp
    DWORD hookFuncAddr = (DWORD)hookedDrawIndexPrimitive;
    DWORD relativeAddress = hookFuncAddr - (DWORD)oDrawIndexedPrim - 5;

    //create jump instruction
    BYTE insts[5] = {0xE9};
    memcpy(&insts[1],&relativeAddress,4);

    DWORD oldProtect;
    VirtualProtect(oDrawIndexedPrim,5,PAGE_EXECUTE_READWRITE, &oldProtect);
    memcpy(oDrawIndexedPrim,&insts,5);
    VirtualProtect(oDrawIndexedPrim, 5, oldProtect, &oldProtect);  // Restore original protection

	GenerateShader(device, &shaderfront, 31.0f / 255.0f, 99.0f / 255.0f, 155.0f / 255.0f);
	GenerateShader(device, &shaderback, 221.0f / 255.0f, 177.0f / 255.0f, 31.0f / 255.0f);

    /*Used with logger*/
    // //Main loop to detect key presses
    // while (true) {
    //     DetectKeyPresses();
    // Sleep(1);
    // }
}

// Thread function
DWORD WINAPI MyThreadFunction(LPVOID lpParam) {
    HWND hWnd = FindWindow(NULL, "Call of Duty 4");
    if (hWnd == NULL) {
        MessageBox(NULL, "Window not found!", "Error", MB_OK);
        return 1;
    }

    LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;


    LPDIRECT3DDEVICE9 device;
    if (SUCCEEDED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                     D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device))) {
        hkDrawIndexPrimitive(device);
    }
    system("pause");
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
                    if (AllocConsole()) {
                FILE* pCStream;
                freopen_s(&pCStream, "CONOUT$", "w", stdout); // Redirect stdout to the new console
                std::cout << "Hello from DLL!" << std::endl;
            } else {
                // Handle error if creating a new console fails
                std::cerr << "Failed to allocate console." << std::endl;
            }

        CreateThread(nullptr, 0, MyThreadFunction, nullptr, 0, nullptr);
    }
    return TRUE;
}