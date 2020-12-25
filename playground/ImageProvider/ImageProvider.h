#pragma once
#include <QImage>
#include <QImageReader>

class ImageProvider {
public:
    enum Format {
        IF_RGB,
        IF_RGBA,
        IF_BGR,
        IF_BGRA,
        IF_Gray
    };

    virtual QImage Read() = 0;
    virtual QImage Read(int x, int y, int w, int h) = 0;

protected:
    int mWidth;
    int mHeight;
    int mChannels;
};
