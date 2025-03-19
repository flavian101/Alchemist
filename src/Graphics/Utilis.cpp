 #include "Utilis.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb image.h"
#include "Graphics.h"
#include "models/Vertex.h"
#include "ErrorEx.h"




Utils::VertexBuffer::VertexBuffer(Graphics& gfx,const std::vector<Vertex>& vertices)
    :
    stride(0),
    pVertexBuffer(nullptr)
{
    if (pVertexBuffer)pVertexBuffer.Reset();
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
    CHECK_RESULT(gfx.GetDevice()->CreateBuffer(&vbDesc, &vbData, pVertexBuffer.GetAddressOf()));
    //vertices.clear();
}

Utils::VertexBuffer::~VertexBuffer()
{
    pVertexBuffer.Reset();
}

void Utils::VertexBuffer::Bind(Graphics& gfx)
{
    gfx.GetContext()->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);
}

void Utils::VertexBuffer::Unbind(Graphics& gfx)
{
    ID3D11Buffer* nullBuffer = nullptr;
    UINT zero = 0;
    gfx.GetContext()->IASetVertexBuffers(0, 1, &nullBuffer, &zero, &zero);
}

Utils::IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices)
    :
    pIndexBuffer(nullptr)
{
    if (pIndexBuffer)pIndexBuffer.Reset();

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

    CHECK_RESULT(gfx.GetDevice()->CreateBuffer(&idDesc, &idData, pIndexBuffer.GetAddressOf()));
    //indices.clear();
}

Utils::IndexBuffer::~IndexBuffer()
{
    pIndexBuffer.Reset();
}

void Utils::IndexBuffer::Bind(Graphics& gfx)
{
    gfx.GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}
void Utils::IndexBuffer::UnBind(Graphics& gfx)
{
    gfx.GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
}
Utils::InputLayout::InputLayout(Graphics& gfx, const std::string& keyword, Microsoft::WRL::ComPtr<ID3DBlob> pVsByteCode)
{
    layouts.clear();
    if (keyword == "POSITION")
    {
        layouts.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    }
    else if (keyword == "POSITION|COLOR")
    {
        layouts.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        layouts.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    }
    else if (keyword == "POSITION|TEXCOORD")
    {
        layouts.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        layouts.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    }
    else if (keyword == "POSITION|TEXCOORD|NORMAL")
    {

        layouts.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        layouts.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        layouts.push_back({ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    }
    else if (keyword == "POSITION|TEXCOORD|NORMAL|TANGENT")
    {
        layouts.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 });
        layouts.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        layouts.push_back({ "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 });
        layouts.push_back({ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT ,0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 });
    }
    else
    {
        MessageBox(nullptr, L"input layout error", L"error", MB_OK);
    }
    {
        D3D11_INPUT_ELEMENT_DESC* layout = layouts.data();
        UINT numElements = static_cast<UINT>(layouts.size());

        CHECK_RESULT(gfx.GetDevice()->CreateInputLayout(layout, numElements, pVsByteCode->GetBufferPointer(),
            pVsByteCode->GetBufferSize(), pInputLayout.GetAddressOf()));

    }
    layouts.clear();
   
}
Utils::InputLayout::~InputLayout()
{
    pInputLayout.Reset();
    layouts.clear();

}

void Utils::InputLayout::Bind(Graphics& gfx)
{
    gfx.GetContext()->IASetInputLayout(pInputLayout.Get());
}

void Utils::InputLayout::UnBind(Graphics& gfx)
{
    gfx.GetContext()->IASetInputLayout(nullptr);
}

Utils::VertexShader::VertexShader(Graphics& gfx,const std::wstring& path)
{
    LoadCompiledVertexShader(gfx, path);
}

Utils::VertexShader::~VertexShader()
{
    pVertexShader.Reset();
    pShaderBlob.Reset();
}

void Utils::VertexShader::LoadStreamVertexShader(Graphics& gfx,std::string stream)
{
    if (pVertexShader)pVertexShader.Reset();
    CHECK_RESULT(D3DCompile(stream.c_str(), stream.size(), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, pShaderBlob.GetAddressOf(), nullptr));
    gfx.GetDevice()->CreateVertexShader(
        pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), nullptr, pVertexShader.GetAddressOf());
    
}

void Utils::VertexShader::LoadCompiledVertexShader(Graphics& gfx,std::wstring path)
{
    if (pVertexShader)pVertexShader.Reset();
    D3DReadFileToBlob(path.c_str(), pShaderBlob.GetAddressOf());
    if (pShaderBlob == NULL)
    {
        MessageBox(NULL, L"empty vertex shader", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
    }
    CHECK_RESULT(gfx.GetDevice()->CreateVertexShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), nullptr, pVertexShader.GetAddressOf()));

}

ID3D10Blob* Utils::VertexShader::GetByteCode()
{
    return pShaderBlob.Get();
}

void Utils::VertexShader::Bind(Graphics& gfx)
{
   // pShaderBlob.Reset();
    gfx.GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0);
}

void Utils::VertexShader::UnBind(Graphics& gfx)
{
    gfx.GetContext()->VSSetShader(nullptr, nullptr, 0);
}

Utils::PixelShader::PixelShader(Graphics& gfx,const std::wstring path)
{
    LoadCompiledPixelShader(gfx, path);
}

Utils::PixelShader::~PixelShader()
{
    pShaderBlob.Reset();
    pPixelShader.Reset();
}

void Utils::PixelShader::LoadStreamPixelShader(Graphics& gfx,std::string stream)
{
    if (pPixelShader)pPixelShader.Reset();
    CHECK_RESULT(D3DCompile(stream.c_str(), stream.size(), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, pShaderBlob.GetAddressOf(), nullptr));
    gfx.GetDevice()->CreatePixelShader(
        pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), nullptr, pPixelShader.GetAddressOf());
}

void Utils::PixelShader::LoadCompiledPixelShader(Graphics& gfx,std::wstring path)
{
    if (pPixelShader)pPixelShader.Reset();
    D3DReadFileToBlob(path.c_str(), &pShaderBlob);
    gfx.GetDevice()->CreatePixelShader(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), nullptr, pPixelShader.GetAddressOf());

}

ID3DBlob* Utils::PixelShader::GetByteCode()
{
    return pShaderBlob.Get();
}

void Utils::PixelShader::Bind(Graphics& gfx)
{
    gfx.GetContext()->PSSetShader(pPixelShader.Get(), nullptr,0);
}
void Utils::PixelShader::Unbind(Graphics& gfx)
{
    gfx.GetContext()->PSSetShader(nullptr, nullptr, 0);
}
Utils::Topology::Topology(Graphics& g, D3D11_PRIMITIVE_TOPOLOGY type)
    :
    type(type)
{}

void Utils::Topology::Bind(Graphics& gfx)
{
    gfx.GetContext()->IASetPrimitiveTopology(type);
}

Utils::Texture::Texture(Graphics& gfx, const char* path)
{
    if (pTex) pTex.Reset();
    if (textureView) textureView.Reset();
    int image_Width, image_height, image_Channels, image_Desired_channels = 4;

    unsigned char* data = stbi_load(path, &image_Width, &image_height,
        &image_Channels, image_Desired_channels);

    if (data == nullptr)
    {
        std::wstring errorMsg = L"Failed to load the texture.\nImage Path: ";
        errorMsg += std::wstring(path, path + strlen(path));
        MessageBox(nullptr, errorMsg.c_str(), L"ERROR", MB_ICONWARNING | MB_OK);
        // MessageBox(nullptr, L"Failed to load the texture", L"ERROR", MB_ICONWARNING | MB_OK);
    }
    try
    {
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
        sts.SysMemPitch = static_cast<UINT>(image_pitch);

        CHECK_RESULT(gfx.GetDevice()->CreateTexture2D(&ts, &sts, pTex.GetAddressOf()));
        // create the resource view on the texture
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = ts.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        CHECK_RESULT(gfx.GetDevice()->CreateShaderResourceView(pTex.Get(), &srvDesc, textureView.GetAddressOf()));
    }
    catch (...)
    {
        stbi_image_free(data);
        throw;
    }
    stbi_image_free(data);
}

Utils::Texture::~Texture()
{
    pTex.Reset();
    textureView.Reset();
}


void Utils::Texture::Bind(Graphics& gfx,UINT slot)
{
    m_slot = slot;
    gfx.GetContext()->PSSetShaderResources(m_slot, 1, textureView.GetAddressOf());

}

void Utils::Texture::UnBind(Graphics& gfx)
{
    ID3D11ShaderResourceView* nullSRV = nullptr;
    gfx.GetContext()->PSSetShaderResources(0, 1, &nullSRV);
}

Utils::Sampler::Sampler(Graphics& gfx)
{
    D3D11_SAMPLER_DESC sp;
    ZeroMemory(&sp, sizeof(sp));

    sp.Filter = D3D11_FILTER_ANISOTROPIC;
    sp.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sp.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sp.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sp.MipLODBias = 0.0f;
    sp.MaxAnisotropy = 4u;
    sp.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sp.MinLOD = 0;
    sp.MaxLOD = D3D11_FLOAT32_MAX;

    CHECK_RESULT(gfx.GetDevice()->CreateSamplerState(&sp, pSampler.GetAddressOf()));


}
Utils::Sampler::~Sampler()
{
    pSampler.Reset();
}
void Utils::Sampler::Bind(Graphics& gfx)
{
    gfx.GetContext()->PSSetSamplers(0, 1, pSampler.GetAddressOf());

}


