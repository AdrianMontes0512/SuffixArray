#ifndef INTERACTIVE_MENU_H
#define INTERACTIVE_MENU_H

#include "PlagiarismDetector.h"
#include <string>
#include <vector>
#include <functional>

// Clase para manejar la interfaz interactiva en terminal
class InteractiveMenu {
public:
    InteractiveMenu();
    void run();

private:
    // Estructura para opciones de menú
    struct MenuItem {
        std::string label;
        std::function<void()> action;
    };

    // Estado de la aplicación
    enum class State {
        MAIN_MENU,
        INPUT_TEXT_1,
        INPUT_TEXT_2,
        SELECT_TEST_CASE,
        SHOW_RESULTS,
        EXIT
    };

    State current_state_;
    std::string text1_;
    std::string text2_;
    int min_match_length_;
    int selected_option_;
    PlagiarismDetector::Report last_report_;

    // Métodos de renderizado
    void clear_screen();
    void print_header(const std::string& title);
    void print_footer();
    void print_centered(const std::string& text, const std::string& color = "");
    
    // Métodos de lógica
    void show_main_menu();
    void handle_input_text(int text_num);
    void show_test_cases();
    void run_analysis();
    void show_results();
    
    // Utilidades
    std::string get_input(const std::string& prompt);
    void wait_for_key();
    
    // Constantes de color
    const std::string RESET = "\033[0m";
    const std::string BOLD = "\033[1m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string WHITE = "\033[37m";
    const std::string BG_BLUE = "\033[44m";
};

#endif // INTERACTIVE_MENU_H
