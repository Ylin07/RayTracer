#ifndef IMAGE_H
#define IMAGE_H

#ifdef _MSC_VER
    #pragma warning (push,0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG

#include "stb_image.h"
#include "utils.h"

class Image{
    public:
        Image(){}

        Image(const char* image_file){
            auto filename = std::string(image_file);
            if (load(filename)) return;
            if (load("images/" + filename)) return;
            if (load("../images/" + filename)) return;
            if (load("../../images/" + filename)) return;
            if (load("../../../images/" + filename)) return;
            if (load("../../../../images/" + filename)) return;
            if (load("../../../../../images/" + filename)) return;
            if (load("../../../../../../images/" + filename)) return;

            std::cerr << "ERROR: Could not load image file " << image_file << "\n";
        }

        ~Image(){
            delete[] bdata;
            STBI_FREE(fdata);
        }

        bool load(const std::string& filename){
            auto n = bytes_per_pixel;
            fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
            if(fdata == nullptr) return false;

            byte_per_scanline = bytes_per_pixel * image_width;
            convert_to_bytes();
            return true;
        }

        int width()  const {return (fdata==nullptr) ? 0 : image_width;}
        int height() const {return (fdata==nullptr) ? 0 : image_height;}

        const unsigned char* pixel_data(int x, int y) const{
            static unsigned char magenta[] = {255,0,255};
            if(bdata==nullptr) return magenta;

            x = clamp(x,0,image_width);
            y = clamp(y,0,image_height);

            return bdata + x*bytes_per_pixel + y*byte_per_scanline;
        }

    private:
        const int bytes_per_pixel = 3;      // 每像素的位数(即通道数)
        float* fdata = nullptr;             // 浮点像素数据
        unsigned char* bdata = nullptr;     // 8bit像素数据
        int image_width = 0;                // 图像宽度
        int image_height = 0;               // 图像高度
        int byte_per_scanline = 0;          // 宽的像素数量

        static int clamp(int x, int low, int high){
            if(x < low) return low;
            if(x < high) return x;
            return high - 1;
        }

        static unsigned char float_to_byte(float value){
            if(value <= 0.0)
                return 0;
            if(value >= 1.0)
                return 255;
            return static_cast<unsigned>(value*256.0);
        }

        void convert_to_bytes(){
            int total_bytes = bytes_per_pixel * image_height * image_width;
            bdata = new unsigned char[total_bytes];

            auto *bptr = bdata;
            auto *fptr = fdata;
            for(auto i=0; i<total_bytes ; i++,fptr++,bptr++)
                *bptr = float_to_byte(*fptr);
        }
};


#ifdef _MSC_VER
    #pragma warning (pop)
#endif

#endif