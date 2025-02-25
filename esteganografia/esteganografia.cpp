#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char**argv) {
  cv::Mat imagemPortadora, imagemEscondida;
  cv::Vec3b valPortadora, valEscondida;
  int nbits = 3;

  imagemPortadora = cv::imread("desafio_esteganografia.png", cv::IMREAD_COLOR);
  if (imagemPortadora.empty()) {
    std::cerr << "Erro ao carregar a imagem!" << std::endl;
    return -1;
  }

  // Inicialize imagemEscondida com o mesmo tamanho e tipo de imagemPortadora
  imagemEscondida = cv::Mat::zeros(imagemPortadora.size(), imagemPortadora.type());

  for (int i = 0; i < imagemPortadora.rows; i++) {
    for (int j = 0; j < imagemPortadora.cols; j++) {

      valPortadora = imagemPortadora.at<cv::Vec3b>(i, j);
      valEscondida = imagemEscondida.at<cv::Vec3b>(i, j);

      valEscondida[0] = valPortadora[0] << (8-nbits);
      valEscondida[1] = valPortadora[1] << (8-nbits);
      valEscondida[2] = valPortadora[2] << (8-nbits);

      imagemEscondida.at<cv::Vec3b>(i, j) = valEscondida;
    }
  }

  cv::imshow("Imagem escondida na esteganografia", imagemEscondida);
  cv::waitKey(0);

  //cv::imwrite("imagem escondida.png", imagemEscondida);
  
  return 0;
}