#include <iostream>
#include <opencv2/opencv.hpp>


cv::Mat troca_regioes(cv::Mat imagem){

    cv::Mat imagem_nova = imagem.clone();
    int n = imagem_nova.rows/2;
    int m = imagem_nova.cols/2;
  
    for(int i = 0;i<n;i++) {
      for(int j = 0;j<m;j++) {
        imagem_nova.at<uchar>(i,j) = imagem.at<uchar>(n+i,j+m);
        imagem_nova.at<uchar>(i+n,j+m) = imagem.at<uchar>(i,j);
        imagem_nova.at<uchar>(i,j+m) = imagem.at<uchar>(i+n,j);
        imagem_nova.at<uchar>(i+n,j) = imagem.at<uchar>(i,j+m);
      }

    }

    return imagem_nova;

}


int main(int, char**) {
  cv::Mat image;

  //image = cv::imread("biel.png", cv::IMREAD_COLOR);
  image = cv::imread("biel.png", cv::IMREAD_GRAYSCALE);
  if (!image.data) std::cout << "nao abriu biel.png" << std::endl;

  cv::namedWindow("janela", cv::WINDOW_NORMAL);

  cv::Mat imagem_trocada = troca_regioes(image);
  cv::imshow("janela", imagem_trocada);
  cv::waitKey();
  return 0;
}
