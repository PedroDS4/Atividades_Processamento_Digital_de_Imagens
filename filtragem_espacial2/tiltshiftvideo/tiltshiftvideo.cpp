#include <opencv2/opencv.hpp>
#include <iostream>

// Função para calcular a máscara de ponderação usando tangente hiperbólica
cv::Mat calculateAlphaMask(int region_center, int height, int decay, int rows, int cols) {
    cv::Mat alpha_mask = cv::Mat(rows, cols, CV_32F, cv::Scalar(0));

    int l1 = region_center - height / 2;
    int l2 = region_center + height / 2;

    for (int y = 0; y < rows; y++) {
        float alpha_value = 0.5f * (std::tanh((y - l1) / static_cast<float>(decay)) - std::tanh((y - l2) / static_cast<float>(decay)));
        alpha_mask.row(y).setTo(alpha_value); // Preenche as linhas com valores calculados
    }

    // Normaliza a máscara para garantir que fique entre 0 e 1
    cv::normalize(alpha_mask, alpha_mask, 0.0, 1.0, cv::NORM_MINMAX);

    // Convertendo a máscara para 3 canais para compatibilidade com imagem RGB
    cv::Mat alpha_mask_3c;
    cv::cvtColor(alpha_mask, alpha_mask_3c, cv::COLOR_GRAY2BGR);

    return alpha_mask_3c;
}

// Função para aplicar o efeito tilt-shift em cada quadro
cv::Mat applyTiltShift(cv::Mat& frame) {
    int region_center = frame.rows / 2 + 100;
    int height = frame.rows / 3;
    int decay = 10;

    // Calcula o mapa de ponderação e converte para 3 canais
    cv::Mat alpha = calculateAlphaMask(region_center, height, decay, frame.rows, frame.cols);
    cv::Mat inverted_alpha = 1.0 - alpha;

    // Converte a imagem para o tipo float com 3 canais
    cv::Mat frame_float, blurred_float;
    frame.convertTo(frame_float, CV_32FC3, 1.0 / 255.0);

    // Aplica o filtro de média para criar a imagem borrada
    cv::Mat blurred_temp;
    int kernel_size = 15;
    cv::blur(frame, blurred_temp, cv::Size(kernel_size, kernel_size));
    blurred_temp.convertTo(blurred_float, CV_32FC3, 1.0 / 255.0);

    // Multiplica as imagens pela máscara
    cv::Mat original_weighted, blurred_weighted;
    cv::multiply(frame_float, alpha, original_weighted);
    cv::multiply(blurred_float, inverted_alpha, blurred_weighted);

    // Combina as imagens ponderadas
    cv::Mat tiltshift_image;
    cv::add(original_weighted, blurred_weighted, tiltshift_image);

    // Clampa valores para evitar saturação
    cv::min(tiltshift_image, 1.0, tiltshift_image);

    // Converte de volta para CV_8UC3 e escala para o intervalo correto
    tiltshift_image.convertTo(tiltshift_image, CV_8UC3, 255.0);
    return tiltshift_image;
}

// Função para processar o vídeo
void processVideo(const std::string& input_video_path) {
    // Abrir o vídeo de entrada
    cv::VideoCapture cap(input_video_path);
    if (!cap.isOpened()) {
        std::cerr << "Erro ao abrir o arquivo de vídeo." << std::endl;
        return;
    }

    // Obter propriedades do vídeo (como o número de quadros por segundo e a resolução)
    double fps = cap.get(cv::CAP_PROP_FPS);
    int frame_width = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int codec = static_cast<int>(cap.get(cv::CAP_PROP_FOURCC));

    // Criar o VideoWriter para o arquivo de saída com nome fixo 'output.mp4'
    cv::VideoWriter output_video("output.mp4", codec, fps, cv::Size(frame_width, frame_height));

    // Verifica se a criação do VideoWriter foi bem-sucedida
    if (!output_video.isOpened()) {
        std::cerr << "Erro ao criar o arquivo de vídeo de saída." << std::endl;
        return;
    }

    // Processa os quadros do vídeo
    cv::Mat frame, tiltshift_frame;
    int frame_counter = 0;
    while (cap.read(frame)) {
        if (frame.empty()) {
            std::cerr << "Quadro vazio detectado!" << std::endl;
            continue; // Se o quadro estiver vazio, continue para o próximo
        }

        // Aplique o efeito de tilt-shift apenas em alguns quadros
        if (frame_counter % 10 == 0) { // Apenas a cada 3 quadros, por exemplo
            tiltshift_frame = applyTiltShift(frame);
            output_video.write(tiltshift_frame); // Escreve o quadro no vídeo de saída
        }
        frame_counter++;
    }

    // Libera os recursos
    cap.release();
    output_video.release();
    std::cout << "Vídeo de saída salvo como 'output.mp4'" << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <caminho_do_video_entrada>" << std::endl;
        return -1;
    }

    // Processa o vídeo e salva como 'output.mp4'
    processVideo(argv[1]);

    return 0;
}

