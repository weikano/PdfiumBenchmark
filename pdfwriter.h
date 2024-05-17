#ifndef PDFWRITER_H
#define PDFWRITER_H

#include "fpdf_save.h"

class PdfWriter : public FPDF_FILEWRITE
{
public:
    PdfWriter();
};

#endif // PDFWRITER_H
