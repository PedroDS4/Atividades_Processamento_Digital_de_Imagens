#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv) {
    cv::VideoCapture cap;

    // Definindo o kernel do filtro Laplaciano
    float laplacian[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};

    cv::Mat frame, framegray, frame32f, laplaciano;
    cv::Mat mask(3, 3, CV_32F);
    cv::Mat f_max;
    cv::Mat max_laplacian;

    // Abre o vídeo
    cap.open(argv[1]);
    if (!cap.isOpened()) return -1;

    // Obter as dimensões do primeiro frame
    cap.read(frame);
    if (frame.empty()) return -1; // Verifica se o frame foi capturado corretamente
    int width = frame.cols;  // Obter a largura do frame
    int height = frame.rows; // Obter a altura do frame

    std::cout << "largura=" << width << "\n";
    std::cout << "altura =" << height << "\n";

    // Criando matrizes para armazenar os resultados
    mask = cv::Mat(3, 3, CV_32F, laplacian);
    f_max = cv::Mat::zeros(frame.size(), CV_8UC3); // Imagem de saída colorida
    max_laplacian = cv::Mat::zeros(frame.size(), CV_32F); // Matriz para armazenar máximos Laplacianos
  
    // Loop para capturar todos os frames do vídeo
    while (true) {
        if (!cap.read(frame)) break; // Enquanto houver frames, o loop continua

        // Converter o frame para tons de cinza
        cv::cvtColor(frame, framegray, cv::COLOR_BGR2GRAY);

        // Converter o frame para float 32 bits.
        framegray.convertTo(frame32f, CV_32F);
        
        // Aplicar o filtro Laplaciano
        cv::filter2D(frame32f, laplaciano, CV_32F, mask, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);

        // Comparar o resultado do filtro com a matriz de máximos e atualizar a imagem de saída
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                // Se o valor do Laplaciano for maior que o máximo armazenado
                if (laplaciano.at<float>(i, j) > max_laplacian.at<float>(i, j)) {
                    max_laplacian.at<float>(i, j) = laplaciano.at<float>(i, j); // Atualiza o máximo
                    f_max.at<cv::Vec3b>(i, j) = frame.at<cv::Vec3b>(i, j); // Copiar o pixel colorido original
                }
            }
        }


        // Exibir a imagem de saída atual
        //cv::imshow("janela", f_max);
        
        // Pressione 'q' para sair do loop
        if (cv::waitKey(30) >= 0) break; // Pausa para exibir o frame e verifica se a tecla foi pressionada
    }
    
    // Exibir a imagem de saída
    cv::imshow("janela", f_max);

    cv::Mat f_max_save;
    f_max.convertTo(f_max_save, CV_8UC3); // Escalar e converter para 8 bits

    cv::imwrite("Imagem Realçada.png", f_max_save); // Salvar a imagem final
    cv::waitKey();
    cv::destroyAllWindows(); // Fechar todas as janelas

    return 0;
}

