#include "../include/material.hpp"

RGBi::RGBi(std::string color) {
  if (color == "background") {
    r=22; g=22; b=22;
  } else if (color == "black") {
    r=0; g=0; b=0;
  } else if (color == "white") {
    r=255; g=255; b=255;
  } else if (color == "random") {
    /* srand(time(0)); */
    r=(int)(rand()%256); g=(int)(rand()%256); b=(int)(rand()%256);
  } else if (color == "orange") {
    r=255; g=140; b=0;
  } else if (color == "purple") {
    r=175; g=35; b=255;
  } else if (color == "green") {
    r=0; g=255; b=0;
  } else {
    r=0; g=0; b=0;
  }
}

RGBi operator + (RGBi color1, RGBi color2) {
  return RGBi(color1.r+color2.r, color1.g+color2.g, color1.b+color2.b);
} 
RGBi operator - (RGBi color1, RGBi color2) {
  return RGBi(color1.r-color2.r, color1.g-color2.g, color1.b-color2.b);
} 
RGBi operator * (RGBi color, int s) {
  return RGBi(color.r*s, color.g*s, color.b*s);
} 
RGBi operator * (RGBi color, float s) {
  return RGBi(color.r*s, color.g*s, color.b*s);
} 
RGBi operator / (RGBi color, int s) {
  return RGBi(color.r/s, color.g/s, color.b/s);
} 
RGBi operator / (RGBi color, float s) {
  return RGBi(color.r/s, color.g/s, color.b/s);
} 

void RGBi::normalize() {
  if (r<0) {r=0;} else if (r>255) {r=255;}
  if (g<0) {g=0;} else if (g>255) {g=255;}
  if (b<0) {b=0;} else if (b>255) {b=255;}
}
void RGBi::invert() {
  normalize();
  r = 255-r; g = 255-g; b = 255-b; 
}
