#ifndef PDFIUMRENDERER_H
#define PDFIUMRENDERER_H

#include "PdfRenderer.h"
#include "fpdf_doc.h"
class PdfiumRenderer : public PdfRenderer
{
public:
    PdfiumRenderer();
    ~PdfiumRenderer();
    void closeDocument() override;
    void openDocument(const char*, const char*) override;
    int getPageCount() override;
    void render(int pageIndex) override;
    void render(int pageIndex, float scale) override;
    void renderScale(int pageIndex, float scale) override;
    void renderMatrix(int pageIndex, float scale) override;
    void renderWithoutBitmap(int pageIndex, float scale) override;
    void renderProgressive(int pageIndex, float scale) override;
    void renderMutliscale(int pageIndex, float scale) override;
    void showbox(int pageIndex) override;
    void saveAs() override;
private:
    FPDF_DOCUMENT document;
};

#endif // PDFIUMRENDERER_H
