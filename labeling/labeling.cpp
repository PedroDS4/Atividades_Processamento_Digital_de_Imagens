#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
  cv::Mat image;
  int width, height;
  int nobjects = 0;
  int nobjects_with_holes = 0;
  cv::Point p;

  // Carrega a imagem em escala de cinza
  image = cv::imread("bolhas.png", cv::IMREAD_GRAYSCALE);

  if (!image.data) {
    std::cout << "imagem nao carregou corretamente\n";
    return (-1);
  }

  width = image.cols;
  height = image.rows;
  std::cout << width << "x" << height << std::endl;

  // Remove objetos conectados às bordas
  for (int i = 0; i < width; i++) {
    if (image.at<uchar>(0, i) == 255) cv::floodFill(image, Point(i, 0), 0);
    if (image.at<uchar>(height - 1, i) == 255) cv::floodFill(image, Point(i, height - 1), 0);
  }
  for (int i = 0; i < height; i++) {
    if (image.at<uchar>(i, 0) == 255) cv::floodFill(image, Point(0, i), 0);
    if (image.at<uchar>(i, width - 1) == 255) cv::floodFill(image, Point(width - 1, i), 0);
  }

  // Conta os objetos e verifica buracos
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (image.at<uchar>(i, j) == 255) {
        // Encontrou uma nova bolha
        nobjects++;

        // Define um ID único para a bolha atual
        p.x = j;
        p.y = i;

        // Marca a bolha com o ID atual
        cv::floodFill(image, p, nobjects);

        // Verifica por buracos dentro da área da bolha
        bool has_holes = false;
        for (int y = i; y < height; y++) {
          for (int x = j; x < width; x++) {
            // Verifica se um ponto preto está cercado pela bolha
            if (image.at<uchar>(y, x) == 0 && 
               (image.at<uchar>(y, x+1) == nobjects)) {

              // Marca o buraco com uma cor temporária para evitar duplicações
              cv::floodFill(image, Point(x, y), 127); // 127 para diferenciar de outras áreas
              has_holes = true;
            }
          }
        }
       

        // Conta a bolha com buraco, se encontrado
        if (has_holes) nobjects_with_holes++; 
      }
    }
  }

  std::cout << "A figura tem " << nobjects << " bolhas que não tocam as bordas e " 
            << nobjects_with_holes << " bolhas com buracos. \n";

  cv::imshow("image", image);
  cv::imwrite("labeling.png", image);
  cv::waitKey();
  return 0;
}
