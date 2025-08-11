//
// repo:            scren_recorder
// file:            main_os_desktop_change_clbk_test.cpp
// path:            src/tests/os_desktop_change_clbk_test/main_os_desktop_change_clbk_test.cpp
// created on:      2025 May 06
// created by:      Davit Kalantaryan (davit.kalantaryan@desy.de)
//

#include <cinternal/disable_compiler_warnings.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl/client.h>
#include <qtutils/disable_utils_warnings.h>
#include <QImage>
#include <QDebug>
#include <QCoreApplication>
#include <cinternal/undisable_compiler_warnings.h>

using Microsoft::WRL::ComPtr;

QImage dxgiTextureToQImage(ID3D11Texture2D* texture, ID3D11Device* device, ID3D11DeviceContext* context) {
    D3D11_TEXTURE2D_DESC desc;
    texture->GetDesc(&desc);

    D3D11_TEXTURE2D_DESC cpuDesc = desc;
    cpuDesc.Usage = D3D11_USAGE_STAGING;
    cpuDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    cpuDesc.BindFlags = 0;
    cpuDesc.MiscFlags = 0;

    ComPtr<ID3D11Texture2D> cpuTex;
    if (FAILED(device->CreateTexture2D(&cpuDesc, nullptr, &cpuTex))) return QImage();

    context->CopyResource(cpuTex.Get(), texture);

    D3D11_MAPPED_SUBRESOURCE mapped;
    if (FAILED(context->Map(cpuTex.Get(), 0, D3D11_MAP_READ, 0, &mapped))) return QImage();

    QImage img(reinterpret_cast<uchar*>(mapped.pData), desc.Width, desc.Height, mapped.RowPitch, QImage::Format_RGB32);
    QImage copy = img.copy();
    context->Unmap(cpuTex.Get(), 0);

    return copy;
}


int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    D3D_FEATURE_LEVEL featureLevel;

    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &device, &featureLevel, &context))) {
        qWarning() << "Failed to create D3D11 device.";
        return -1;
    }

    ComPtr<IDXGIDevice> dxgiDevice;
    device.As(&dxgiDevice);

    ComPtr<IDXGIAdapter> adapter;
    dxgiDevice->GetAdapter(&adapter);

    ComPtr<IDXGIOutput> output;
    adapter->EnumOutputs(0, &output);

    ComPtr<IDXGIOutput1> output1;
    output.As(&output1);

    ComPtr<IDXGIOutputDuplication> duplication;
    if (FAILED(output1->DuplicateOutput(device.Get(), &duplication))) {
        qWarning() << "Failed to duplicate output.";
        return -2;
    }

    DXGI_OUTDUPL_FRAME_INFO frameInfo = {};
    ComPtr<IDXGIResource> desktopResource;

    HRESULT hr = duplication->AcquireNextFrame(1000, &frameInfo, &desktopResource);
    if (SUCCEEDED(hr)) {
        ComPtr<ID3D11Texture2D> acquiredTex;
        desktopResource.As(&acquiredTex);

        QImage image = dxgiTextureToQImage(acquiredTex.Get(), device.Get(), context.Get());
        if (!image.isNull()) {
            QString filename = "captured_frame.png";
            if (image.save(filename)) {
                qDebug() << "Frame captured and saved to" << filename;
            }
            else {
                qWarning() << "Failed to save image.";
            }
        }
        else {
            qWarning() << "Failed to convert frame to image.";
        }

        duplication->ReleaseFrame();
    }
    else {
        qWarning() << "No frame acquired.";
    }

    return 0;
}
