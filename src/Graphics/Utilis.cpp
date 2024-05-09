#include "Utilis.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb image.h"
#include <codecvt>


Utils::VertexBuffer::VertexBuffer(Graphics& g)
    :
    m_graphics(g),
    stride(0),
    pVertexBuffer(nullptr)
{}

void Utils::VertexBuffer::InitializeVertexBuffer(std::vector<Vertex>& vertices)
{
    stride = sizeof(Vertex);
    D3D11_BUFFER_DESC vbDesc;
    ZeroMemory(&vbDesc, sizeof(vbDesc));
    vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbDesc.Usage = D3D11_USAGE_DEFAULT;
    vbDesc.CPUAccessFlags = 0u;
    vbDesc.MiscFlags = 0u;
    UINT size = vertices.size() * sizeof(Vertex);
    vbDesc.ByteWidth = size;
    vbDesc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA vbData = {};
    ZeroMemory(&vbData, sizeof(vbData));
    vbData.pSysMem = vertices.data();
    vbData.SysMemPitch = 0;
    vbData.SysMemSlicePitch = 0;
    CHECK_RESULT(m_graphics.GetDevice()->CreateBuffer(&vbDesc, &vbData, pVertexBuffer.GetAddressOf()));

}

void Utils::VertexBuffer::Bind()
{
    m_graphics.GetContext()->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);
}

Utils::IndexBuffer::IndexBuffer(Graphics& g)
    :
    m_graphics(g),
    pIndexBuffer(nullptr)
{

    
}

void Utils::IndexBuffer::InitializeIndexBuffer(std::vector<unsigned short>& indices)
{
    D3D11_BUFFER_DESC idDesc;
    ZeroMemory(&idDesc, sizeof(idDesc));
    idDesc.Usage = D3D11_USAGE_DEFAULT;
    idDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    idDesc.CPUAccessFlags = 0;
    idDesc.MiscFlags = 0;
    UINT stride = indices.size() * sizeof(unsigned short);
    idDesc.ByteWidth = stride;
    // idDesc.ByteWidth = indices.size() *sizeof(unsigned short);
    idDesc.StructureByteStride = sizeof(unsigned short);

    D3D11_SUBRESOURCE_DATA idData{};
    idData.pSysMem = indices.data();

    CHECK_RESULT(m_graphics.GetDevice()->CreateBuffer(&idDesc, &idData, pIndexBuffer.GetAddressOf()));
    // Ensure pIndexBuffer ComPtr is valid
    if (pIndexBuffer == nullptr) {
        // Output error message if pIndexBuffer is null
        OutputDebugString(L"pIndexBuffer is null\n");
        return;
    }
}

void Utils::IndexBuffer::Bind()
{
    m_graphics.GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}
Utils::InputLayout::InputLayout(Graphics& g)
    :
    m_graphics(g)
{
   
}
void Utils::InputLayout::CreateLayout(const D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements, Microsoft::WRL::ComPtr<ID3DBlob> pVsByteCode)
{
    CHECK_RESULT(m_graphics.GetDevice()->CreateInputLayout(layoutDesc, numElements, pVsByteCode->GetBufferPointer(),
        pVsByteCode->GetBufferSize(), pInputLayout.GetAddressOf()));

}

void Utils::InputLayout::Bind()
{
    m_graphics.GetContext()->IASetInputLayout(pInputLayout.Get());
}

Utils::VertexShader::VertexShader(Graphics& g)
    :
    m_graphics(g)
{}

void Utils::VertexShader::LoadVertexShader(LPCWSTR path)
{
    D3DReadFileToBlob(path, pShaderBlob.GetAddressOf());
    if (pShaderBlob == NULL)
    {
        MessageBox(NULL, L"empty vertex shader", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
    }
    CHECK_RESULT(m_graphics.GetDevice()->CreateVertexShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), nullptr, pVertexShader.GetAddressOf()));

}

ID3D10Blob* Utils::VertexShader::GetByteCode()
{
    return pShaderBlob.Get();
}

void Utils::VertexShader::Bind()
{
    m_graphics.GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0);
}

Utils::PixelShader::PixelShader(Graphics& g)
    :
    m_graphics(g)
{
}

void Utils::PixelShader::LoadPixelShader(LPCWSTR path)
{
    D3DReadFileToBlob(path, &pShaderBlob);
    m_graphics.GetDevice()->CreatePixelShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), nullptr, pPixelShader.GetAddressOf());

}

void Utils::PixelShader::Bind()
{
    m_graphics.GetContext()->PSSetShader(pPixelShader.Get(), nullptr,0);
}
Utils::Topology::Topology(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY type)
    :
    type(type),
    m_graphics(g)
{}

void Utils::Topology::Bind()
{
    m_graphics.GetContext()->IASetPrimitiveTopology(type);
}

Utils::Texture::Texture(Graphics& g)
    :
    m_graphics(g)
{}

void Utils::Texture::LoadTexture(const char* path, UINT slot)
{
    m_slot = slot;
    int image_Width, image_height , image_Channels, image_Desired_channels = 4;

    unsigned char* data = stbi_load(path, &image_Width, &image_height,
        &image_Channels, image_Desired_channels);

    if (data == NULL)
    {
      //  std::wstring errorMsg = L"Failed to load the texture";
        //errorMsg += std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(path);
       // MessageBox(nullptr, errorMsg.c_str(), L"ERROR", MB_ICONWARNING | MB_OK);
        MessageBox(nullptr, L"Failed to load the texture", L"ERROR", MB_ICONWARNING | MB_OK);
    }
    int image_pitch = image_Width * 4;

    D3D11_TEXTURE2D_DESC ts = {};
    ts.Width = image_Width;
    ts.Height = image_height;
    ts.MipLevels = 1;
    ts.ArraySize = 1;
    ts.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    ts.SampleDesc.Count = 1;
    ts.SampleDesc.Quality = 0;
    ts.Usage = D3D11_USAGE_DEFAULT;
    ts.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    ts.CPUAccessFlags = 0;
    ts.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sts = {};
    sts.pSysMem = data;
    sts.SysMemPitch = image_pitch;

    CHECK_RESULT(m_graphics.GetDevice()->CreateTexture2D(&ts, &sts, pTex.GetAddressOf()));
    // create the resource view on the texture
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = ts.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    CHECK_RESULT(m_graphics.GetDevice()->CreateShaderResourceView(pTex.Get(), &srvDesc, textureView.GetAddressOf()));

    stbi_image_free(data);
}

void Utils::Texture::Bind()
{
    m_graphics.GetContext()->PSSetShaderResources(m_slot, 1, textureView.GetAddressOf());

}

Utils::Sampler::Sampler(Graphics& g)
    :
    m_graphics(g)
{
    D3D11_SAMPLER_DESC sp;
    ZeroMemory(&sp, sizeof(sp));

    sp.Filter = D3D11_FILTER_ANISOTROPIC;
    sp.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sp.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sp.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sp.MipLODBias = 0.0f;
    sp.MaxAnisotropy = 16;
    sp.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    sp.MinLOD = 0;
    sp.MaxLOD = D3D11_FLOAT32_MAX;

    CHECK_RESULT(g.GetDevice()->CreateSamplerState(&sp, pSampler.GetAddressOf()));


}
void Utils::Sampler::Bind()
{
    m_graphics.GetContext()->PSSetSamplers(0, 1, pSampler.GetAddressOf());

}


