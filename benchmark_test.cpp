#include <benchmark/benchmark.h>
#include "pdfiumrenderer.h"

#include <filesystem>

#include "custommemorymanager.h"
namespace fs = std::filesystem;

static void BM_Pdfium(benchmark::State& state)
{
    printf("VERSION %s\n", PDFIUM_VERSION_STR);
    auto renderer = new PdfiumRenderer;
    std::vector<std::string> files;
    for(const auto& entry : fs::directory_iterator(RESOURCE_DIR)) {
        files.push_back(entry.path());
    }
    for(auto _ : state)
    {
        for(const auto& file : files)
        {
            printf("VERSION %s\n", file.c_str());
            int pageCount = renderer->getPageCount();
            for(int i=0;i<pageCount;i++)
            {
                renderer->render(i);
            }
            renderer->closeDocument();
        }
    }
    delete renderer;
}

static void BM_Pdfium_render_scale(benchmark::State& state)
{
    auto renderer = new PdfiumRenderer;
    auto scale = state.range(0);
    std::vector<std::string> files;
    for(const auto& entry : fs::directory_iterator(RESOURCE_DIR)) {
        files.push_back(entry.path());
    }
    for(auto _ : state)
    {
        for(const auto& file : files)
        {
            renderer->openDocument(file.c_str(), "");
            renderer->render(0, scale);
            renderer->closeDocument();
        }
    }
    delete renderer;
    state.SetBytesProcessed(int64_t(state.iterations()*int64_t(state.range(0))));
}

static void BM_Pdfium_render_1(benchmark::State& state)
{

    std::vector<std::string> files;
    for(const auto& entry : fs::directory_iterator(RESOURCE_DIR)) {
        files.push_back(entry.path());
    }
    for(auto _ : state)
    {
        for(const auto& file : files)
        {
            auto renderer = new PdfiumRenderer;
            renderer->openDocument(file.c_str(), "");
            renderer->render(0);
            renderer->closeDocument();
            delete renderer;
        }
    }
}

static void BM_Pdfium_render_2(benchmark::State& state)
{

    std::vector<std::string> files;
    for(const auto& entry : fs::directory_iterator(RESOURCE_DIR)) {
        files.push_back(entry.path());
    }
    for(auto _ : state)
    {
        for(const auto& file : files)
        {
            auto renderer = new PdfiumRenderer;
            renderer->openDocument(file.c_str(), "");
            renderer->render(0,2);
            renderer->closeDocument();
            delete renderer;
        }
    }
}

static void BM_Pdfium_render_4(benchmark::State& state)
{

    std::vector<std::string> files;
    for(const auto& entry : fs::directory_iterator(RESOURCE_DIR)) {
        files.push_back(entry.path());
    }
    for(auto _ : state)
    {
        for(const auto& file : files)
        {
            auto renderer = new PdfiumRenderer;
            renderer->openDocument(file.c_str(), "");
            renderer->render(0,4);
            renderer->closeDocument();
            delete renderer;
        }
    }
}


static void BM_Pdfium_render_1_5(benchmark::State& state)
{

    std::vector<std::string> files;
    for(const auto& entry : fs::directory_iterator(RESOURCE_DIR)) {
        files.push_back(entry.path());
    }
    for(auto _ : state)
    {
        for(const auto& file : files)
        {
            auto renderer = new PdfiumRenderer;
            renderer->openDocument(file.c_str(), "");
            renderer->render(0,1.5);
            renderer->closeDocument();
            delete renderer;
        }
    }
}

static void BM_Pdfium_render_0_8(benchmark::State& state)
{

    std::vector<std::string> files;
    for(const auto& entry : fs::directory_iterator(RESOURCE_DIR)) {
        files.push_back(entry.path());
    }
    for(auto _ : state)
    {
        for(const auto& file : files)
        {
            auto renderer = new PdfiumRenderer;
            renderer->openDocument(file.c_str(), "");
            renderer->render(0,0.8);
            renderer->closeDocument();
            delete renderer;
        }
    }
}

static void BM_Pdfium_render_0_5(benchmark::State& state)
{

    std::vector<std::string> files;
    for(const auto& entry : fs::directory_iterator(RESOURCE_DIR)) {
        files.push_back(entry.path());
    }
    for(auto _ : state)
    {
        for(const auto& file : files)
        {
            auto renderer = new PdfiumRenderer;
            renderer->openDocument(file.c_str(), "");
            renderer->render(0,0.5);
            renderer->closeDocument();
            delete renderer;
        }
    }
}

static void BM_Pdfium_render_0_2(benchmark::State& state)
{

    std::vector<std::string> files;
    for(const auto& entry : fs::directory_iterator(RESOURCE_DIR)) {
        files.push_back(entry.path());
    }
    for(auto _ : state)
    {
        for(const auto& file : files)
        {
            auto renderer = new PdfiumRenderer;
            renderer->openDocument(file.c_str(), "");
            renderer->render(0,0.2);
            renderer->closeDocument();
            delete renderer;
        }
    }
}

static void BM_Pdfium_render_0_6(benchmark::State& state)
{

    std::vector<std::string> files;
    for(const auto& entry : fs::directory_iterator(RESOURCE_DIR)) {
        files.push_back(entry.path());
    }
    for(auto _ : state)
    {
        for(const auto& file : files)
        {
            auto renderer = new PdfiumRenderer;
            renderer->openDocument(file.c_str(), "");
            renderer->render(0,0.6);
            renderer->closeDocument();
            delete renderer;
        }
    }
}

static void BM_Pdfium_render_8(benchmark::State& state)
{

    std::vector<std::string> files;
    for(const auto& entry : fs::directory_iterator(RESOURCE_DIR)) {
        files.push_back(entry.path());
    }
    for(auto _ : state)
    {
        for(const auto& file : files)
        {
            auto renderer = new PdfiumRenderer;
            renderer->openDocument(file.c_str(), "");
            renderer->render(0,8);
            renderer->closeDocument();
            delete renderer;
        }
    }
}

BENCHMARK(BM_Pdfium_render_1)->Iterations(1);

std::unique_ptr<CustomMemoryManager> mm(new CustomMemoryManager());

#ifdef MEMORY_PROFILER
void *custom_malloc(size_t size) {
    void *p = malloc(size);
    mm.get()->num_allocs += 1;
    mm.get()->max_bytes_used += size;
    return p;
}
#define malloc(size) custom_malloc(size)
#endif



BENCHMARK_MAIN();
//int main(int argc, char** argv)
//{
//    ::benchmark::RegisterMemoryManager(mm.get());
//    ::benchmark::Initialize(&argc, argv);
//    ::benchmark::RunSpecifiedBenchmarks();
//    ::benchmark::RegisterMemoryManager(nullptr);
//}



//BENCHMARK(BM_Pdfium_render_0_2)->Iterations(10);
//BENCHMARK(BM_Pdfium_render_0_5)->Iterations(10);
//BENCHMARK(BM_Pdfium_render_0_6)->Iterations(10);
//BENCHMARK(BM_Pdfium_render_0_8)->Iterations(10);
//BENCHMARK(BM_Pdfium_render_1)->Iterations(10);
//BENCHMARK(BM_Pdfium_render_1_5)->Iterations(10);
//BENCHMARK(BM_Pdfium_render_2)->Iterations(10);
//BENCHMARK(BM_Pdfium_render_4)->Iterations(10);
//BENCHMARK(BM_Pdfium_render_8)->Iterations(10);

//BENCHMARK(BM_Pdfium_render_scale)->Iterations(10)->Arg(16);

//BENCHMARK(BM_Pdfium)->Iterations(10);

//BENCHMARK_MAIN();
