#include "pdfiumrenderer.h"
#include "cpp/fpdf_scopers.h"
#include "fpdf_progressive.h"
#include "fpdf_save.h"
#include <malloc.h>
#include <iostream>
#include <chrono>

PdfiumRenderer::PdfiumRenderer()
{
    FPDF_InitLibrary();
#ifdef PDFIUM_PARTITIONALLOC
    printf("use partialalloc\n");
#else
    printf(" did not use partialalloc\n");
#endif
}

PdfiumRenderer::~PdfiumRenderer()
{

    FPDF_DestroyLibrary();
}

void PdfiumRenderer::closeDocument()
{
    printf("close document\n");
    FPDF_CloseDocument(document);
}

void PdfiumRenderer::openDocument(const char *path, const char *pwd)
{    
    printf("open document %s\n", path);
    document = FPDF_LoadDocument(path, pwd);
}

int PdfiumRenderer::getPageCount()
{
    printf("getPageCount \n ");
    return FPDF_GetPageCount(document);
}

struct rgb {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

struct argb {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

void PdfiumRenderer::render(int pageIndex)
{    
    printf("renderPage %d\n", pageIndex);
    renderMatrix(pageIndex, 1);
}
void PdfiumRenderer::render(int pageIndex, float scale)
{
    renderProgressive(pageIndex, scale);
//    if(scale < 1) {
//        renderScale(pageIndex, scale);
//    }else {
//        renderProgressive(pageIndex, scale);
//    }
//    renderProgressive(pageIndex, scale);
//    renderMatrix(pageIndex, scale);
}

void PdfiumRenderer::renderProgressive(int pageIndex, float scale)
{
    ScopedFPDFPage page(FPDF_LoadPage(document, pageIndex));
    FS_SIZEF size {0,0};
    FPDF_GetPageSizeByIndexF(document, pageIndex, &size);
    size.width *= scale;
    size.height *= scale;
    void* tmp = malloc(size.width * size.height * sizeof(argb));
    ScopedFPDFBitmap bitmap(FPDFBitmap_CreateEx(size.width, size.height, FPDFBitmap_Gray, tmp, size.width * sizeof(argb)));
    FPDFBitmap_FillRect(bitmap.get(), 0,0, size.width, size.height, 0xFFFFFFFF);

    int state = FPDF_RenderPageBitmap_Start(bitmap.get(), page.get(), 0, 0, size.width, size.height, 0, FPDF_REVERSE_BYTE_ORDER | FPDF_ANNOT, NULL);
    printf("FPDF_RenderPageBitmap_Start state:%d\n", state);
    if(state == FPDF_RENDER_TOBECONTINUED || state == FPDF_RENDER_READY) {
        while(FPDF_RenderPage_Continue(page.get(), NULL) != FPDF_RENDER_DONE) {

        }
    }
    FPDF_RenderPage_Close(page.get());
    free(tmp);
    FPDFBitmap_Destroy(bitmap.get());
}

void PdfiumRenderer::renderWithoutBitmap(int pageIndex, float scale)
{
    ScopedFPDFPage page(FPDF_LoadPage(document, pageIndex));
    FS_SIZEF size {0,0};
    FPDF_GetPageSizeByIndexF(document, pageIndex, &size);
    size.width *= scale;
    size.height *= scale;
    ScopedFPDFBitmap bitmap(FPDFBitmap_Create(size.width, size.height, 0));
    FPDFBitmap_FillRect(bitmap.get(), 0,0, size.width, size.height, 0xFFFFFFFF);
    FPDF_RenderPageBitmap(bitmap.get(), page.get(), 0,0, size.width, size.height, 0, FPDF_REVERSE_BYTE_ORDER | FPDF_ANNOT);
}

void PdfiumRenderer::renderScale(int pageIndex, float scale)
{
    ScopedFPDFPage page(FPDF_LoadPage(document, pageIndex));
    FS_SIZEF size {0,0};
    FPDF_GetPageSizeByIndexF(document, pageIndex, &size);
    size.width *= scale;
    size.height *= scale;
    void* tmp = malloc(size.width * size.height * sizeof(argb));
    ScopedFPDFBitmap bitmap(FPDFBitmap_CreateEx(size.width, size.height, FPDFBitmap_Gray, tmp, size.width * sizeof(argb)));
    FPDFBitmap_FillRect(bitmap.get(), 0,0, size.width, size.height, 0xFFFFFFFF);
    FPDF_RenderPageBitmap(bitmap.get(), page.get(), 0,0, size.width, size.height, 0, FPDF_REVERSE_BYTE_ORDER | FPDF_ANNOT);
    free(tmp);
}

void PdfiumRenderer::renderMatrix(int pageIndex, float scale)
{
    ScopedFPDFPage page(FPDF_LoadPage(document, pageIndex));
    float page_width = FPDF_GetPageWidthF(page.get());
    float page_height = FPDF_GetPageHeightF(page.get());
    page_width *= scale;
    page_height *= scale;

    FS_RECTF page_rect{0, 0, page_width, page_width};
    FS_MATRIX matrix{scale, 0, 0, scale, 0, 0};

    void* tmp = malloc(page_width * page_height * sizeof(argb));
    ScopedFPDFBitmap bitmap(FPDFBitmap_CreateEx(page_width , page_height , FPDFBitmap_Gray, tmp, page_width * sizeof(argb)));
    FPDFBitmap_FillRect(bitmap.get(), 0, 0, page_width, page_height,
                            0xFFFFFFFF);
    FPDF_RenderPageBitmapWithMatrix(bitmap.get(), page.get(), &matrix, &page_rect, 0);
    free(tmp);
}

void PdfiumRenderer::showbox(int pageIndex)
{
    ScopedFPDFPage page(FPDF_LoadPage(document, pageIndex));
    float l, t, r, b;
    FPDFPage_GetCropBox(page.get(), &l, &b, &r, &t);
    printf("pdfium pageIndex:%d, cropbox(%f, %f, %f, %f)\n", pageIndex, l, t, r, b);
    FPDFPage_GetMediaBox(page.get(), &l, &b, &r, &t);
    printf("pdfium pageIndex:%d, mediabox(%f, %f, %f, %f)\n", pageIndex, l, t, r, b);
    FPDFPage_GetTrimBox(page.get(), &l, &b, &r, &t);
    printf("pdfium pageIndex:%d, trimbox(%f, %f, %f, %f)\n", pageIndex, l, t, r, b);
}

static void renderUsingScale(FPDF_PAGE page, float page_width, float page_height, float scale)
{
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;
    high_resolution_clock::time_point beginTime = high_resolution_clock::now();
//    float dstWidth = page_width * scale;
//    float dstHeight = page_height * scale;
    int count = 1;
    for(int i=0;i<count;i++) {
        FPDF_BITMAP bitmap = FPDFBitmap_Create(page_width, page_height, 0);
        FPDFBitmap_FillRect(bitmap, 0, 0, page_width, page_height, 0xFFFFFFFF);
        FS_MATRIX matrix {scale, 0, 0, scale, 0, 0};
        FS_RECTF clip {0,0,page_width, page_height};
        FPDF_RenderPageBitmapWithMatrix(bitmap, page, &matrix, &clip, FPDF_REVERSE_BYTE_ORDER | FPDF_ANNOT | FPDF_RENDER_LIMITEDIMAGECACHE | FPDF_PRINTING);

//        FPDF_RenderPageBitmap(bitmap, page, 100, 50, dstWidth, dstHeight, 0, FPDF_REVERSE_BYTE_ORDER | FPDF_ANNOT);
        FPDFBitmap_Destroy(bitmap);
    }
    high_resolution_clock::time_point end = high_resolution_clock::now();
    milliseconds timeInterval = std::chrono::duration_cast<milliseconds>(end - beginTime);
    std::cout<<"scale:"<<scale<<", time cost:"<<timeInterval.count()/count<<std::endl;
}

void PdfiumRenderer::renderMutliscale(int pageIndex, float scale)
{

    ScopedFPDFPage page(FPDF_LoadPage(document, pageIndex));
    float width = FPDF_GetPageWidthF(page.get());
    float height = FPDF_GetPageHeightF(page.get());

    renderUsingScale(page.get(), width, height, 1.0);
    renderUsingScale(page.get(), width, height, 1.0);
    renderUsingScale(page.get(), width, height, 1.0);
    renderUsingScale(page.get(), width, height, 1.0);
    renderUsingScale(page.get(), width, height, 1.0);
    renderUsingScale(page.get(), width, height, 1.0);
    renderUsingScale(page.get(), width, height, 0.5);
    renderUsingScale(page.get(), width, height, 0.8);

    renderUsingScale(page.get(), width, height, 1.2);
    renderUsingScale(page.get(), width, height, 1.5);
    renderUsingScale(page.get(), width, height, 2.0);
    renderUsingScale(page.get(), width, height, 4.0);
    renderUsingScale(page.get(), width, height, 8.0);


}

void PdfiumRenderer::saveAs()
{
//    const char*path = "test.pdf";
//    ScopedFPDFDocument
//    FPDF_SaveAsCopy(document, )

}

