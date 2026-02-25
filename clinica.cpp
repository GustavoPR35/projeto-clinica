#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>

// Feito em 06/2024

using namespace std;

class Paciente {
    public:
        //atributos
        int codigo;
        string nome;
        string endereco;
        string telefone;
        string dataDeNascimento;

        //construtores
        Paciente():
            codigo(0), nome(""), endereco(""), telefone(""), dataDeNascimento("") {};

        Paciente(int codigo, string nome, string endereco, string telefone, string dataDeNascimento):
            codigo(codigo), nome(nome), endereco(endereco), telefone(telefone), dataDeNascimento(dataDeNascimento) {};
};

class Medico {
    public:
        //atributos
        int codigo;
        string nome;
        string telefone;
        string especialidade;

        //construtores
        Medico():
            codigo(0), nome(""), telefone(""), especialidade("") {};
        
        Medico(int codigo, string nome, string telefone, string especialidade):
            codigo(codigo), nome(nome), telefone(telefone), especialidade(especialidade) {};
};

class Consulta {
    public:
        //atributos
        int codigo;
        string data;
        string hora;
        int codigoMedico;
        int codigoPaciente;

        //construtores
        Consulta():
            codigo(0), data(""), hora(""), codigoMedico(0), codigoPaciente(0) {};

        Consulta(int codigo, string data, string hora, int codMed, int codPac):
            codigo(codigo), data(data), hora(hora), codigoMedico(codMed), codigoPaciente(codPac) {};
};

//vetores pros pacientes, medicos e consultas
vector<Paciente> listaDePacientes;
vector<Medico> listaDeMedicos;
vector<Consulta> listaDeConsultas;

void inicializarArquivo(const string& nomeArquivo, int valorInicial) {
    ofstream arquivo(nomeArquivo);
    if (arquivo) {
        arquivo << valorInicial;
    }
    arquivo.close();
}

// Initialize files if they don't exist with default values
void inicializarArquivos() {
    inicializarArquivo("ultimo_codigo_paciente.txt", 1);
    inicializarArquivo("ultimo_codigo_medico.txt", 1);
    inicializarArquivo("ultimo_codigo_consulta.txt", 1);
}

// Function to read the last used code from a file
int lerUltimoCodigo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    int ultimoCodigo = 0;
    if (arquivo) {
        arquivo >> ultimoCodigo;
    }
    arquivo.close();
    return ultimoCodigo;
}

// Function to save/update the last used code in a file
void salvarUltimoCodigo(int novoCodigo, const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    if (arquivo) {
        arquivo << novoCodigo;
    }
    arquivo.close();
}

int CodsPaciente() {
    static int CodPaciente = lerUltimoCodigo("ultimo_codigo_paciente.txt");
    CodPaciente++;
    salvarUltimoCodigo(CodPaciente, "ultimo_codigo_paciente.txt");
    return CodPaciente;
}

int CodsMedico() {
    static int CodMedico = lerUltimoCodigo("ultimo_codigo_medico.txt");
    CodMedico++;
    salvarUltimoCodigo(CodMedico, "ultimo_codigo_medico.txt");
    return CodMedico;
}

int CodsConsultas() {
    static int CodConsulta = lerUltimoCodigo("ultimo_codigo_consulta.txt");
    CodConsulta++;
    salvarUltimoCodigo(CodConsulta, "ultimo_codigo_consulta.txt");
    return CodConsulta;
}

void salvarPaciente(const Paciente &paciente) {
    ofstream arquivoPacientes("pacientes.txt", ios::app);
    arquivoPacientes << paciente.codigo << ";" << paciente.nome << ";" << paciente.endereco << ";"
                     << paciente.telefone << ";" << paciente.dataDeNascimento << endl;
    arquivoPacientes.close();
}

void salvarMedico(const Medico &medico) {
    ofstream arquivoMedicos("medicos.txt", ios::app);
    arquivoMedicos << medico.codigo << ";" << medico.nome << ";" << medico.telefone << ";"
                   << medico.especialidade << endl;
    arquivoMedicos.close();
}

void salvarConsulta(const Consulta &consulta) {
    ofstream arquivoConsultas("consultas.txt", ios::app);
    arquivoConsultas << consulta.codigo << ";" << consulta.data << ";" << consulta.hora << ";"
                     << consulta.codigoMedico << ";" << consulta.codigoPaciente << endl;
    arquivoConsultas.close();
}

void cadastroDoPaciente() {
    int codigo = CodsPaciente();
    string nome;
    string endereco;
    string telefone;
    string dataDeNascimento;

    cout << "Iniciando cadastro de paciente..." << endl;
    cout << "Nome do paciente: ";
    getline(cin, nome);
    cout << "Endereco do paciente: ";
    getline(cin, endereco);
    cout << "Numero de telefone do paciente: ";
    getline(cin, telefone);
    cout << "Data de nascimento do paciente (no formato dia/mes/ano): ";
    getline(cin, dataDeNascimento);

    Paciente paciente(codigo, nome, endereco, telefone, dataDeNascimento);
        listaDePacientes.push_back(paciente);

    salvarPaciente(paciente);
    
    cout << "Paciente cadastrado." << endl;
}

void cadastroDoMedico() {
    int codigo = CodsMedico();
    string nome;
    string telefone;
    string especialidade;

    cout << "Iniciando cadastro de medico..." << endl;
    cout << "Nome do medico: ";
    getline(cin, nome);
    cout << "Numero de telefone do medico: ";
    getline(cin, telefone);
    cout << "Especialidade do medico: ";
    getline(cin, especialidade);

    Medico medico(codigo, nome, telefone, especialidade);
        listaDeMedicos.push_back(medico);
    
    salvarMedico(medico);

    cout << "Medico cadastrado." << endl;
}

//const pra evitar mudanças acidentais de valores nas variaveis, se for possivel
bool verificaDisponibilidade(int codigoMedico, const string &data, const string &hora) {
    int consultasNoDia = 0;
    //tipo novo: "auto", q vai detectar se é int, float, double etc.
    for (const auto &c : listaDeConsultas) { //range-based for loop q parece o do python, "consulta" é basicamente o "i", e depois do ":" é o vetor q vc quer usar
        if (c.codigoMedico == codigoMedico && c.data == data) {
            consultasNoDia++;

            //stoi de string pra int
            //substr pega os caracteres especificados
            //no caso de hora.substr(0, 2), vai iniciar na posição 0 e pegar 2 caracteres a partir de la (serão as horas, HH:mm)
            //hora.substr(3, 2) vai ser os minutos (hh:MM)
            //é igual vetor, começa com posição 0
            int novaHora = stoi(hora.substr(0, 2)) * 60 + stoi(hora.substr(3, 2));
            int consultaHora = stoi(c.hora.substr(0, 2)) * 60 + stoi(c.hora.substr(3, 2));

            int intervalo = 30;

            if (abs(novaHora - consultaHora) < intervalo) {
                return false;
            }
        }
    }
    return consultasNoDia < 2;

    //esse return é basicamente um if:
    //if (consultasNoDia < 2) {
    //    return true;
    //}
}

bool entradaEhNumero(const string &entrada) {
    return !entrada.empty() && all_of(entrada.begin(), entrada.end(), ::isdigit);
}

bool validaData(const string &data) {
    if (data.length() != 10 || data[2] != '/' || data[5] != '/') {
        return false;
    }

    stringstream ss(data);
    string diaStr, mesStr, anoStr;
    getline(ss, diaStr, '/');
    getline(ss, mesStr, '/');
    getline(ss, anoStr);

    if (!entradaEhNumero(diaStr) || !entradaEhNumero(mesStr) || !entradaEhNumero(anoStr)) {
        return false;
    }

    int dia = stoi(diaStr);
    int mes = stoi(mesStr);
    int ano = stoi(anoStr);

    if (mes < 1 || mes > 12 || dia < 1 || dia > 31 || ano < 0) {
        return false;
    }

    return true;
}

bool validaHora(const string &hora) {
    if (hora.length() != 5 || hora[2] != ':') {
        return false;
    }

    stringstream ss(hora);
    string horaStr, minutoStr;
    getline(ss, horaStr, ':');
    getline(ss, minutoStr);

    if (!entradaEhNumero(horaStr) || !entradaEhNumero(minutoStr)) {
        return false;
    }

    int h = stoi(horaStr);
    int m = stoi(minutoStr);

    if (h < 0 || h > 23 || m < 0 || m > 59) {
        return false;
    }

    return true;
}

void agendamentoDaConsulta() {
    int codigo = CodsConsultas();
    string input;
    int codigoPaciente, codigoMedico;
    string data, hora;

    cout << "Iniciando agendamento de consulta..." << endl;
    cout << "Codigo do paciente: ";
    getline(cin, input);
    if (!entradaEhNumero(input)) {
        cout << "Entrada invalida. Por favor, insira um codigo numerico." << endl;
        return;
    }
    codigoPaciente = stoi(input);

    //find_if da biblioteca <algorithm>, q vai ser tipo um for
    //find_if(começo do intervalo, fim do intervalo, lambda expression) { corpo }
    //a lambda expression vai fzr a comparação usando o [codigoPaciente], e (const Paciente &p) vai ser tipo o "i" do for
    auto paciente = find_if(listaDePacientes.begin(), listaDePacientes.end(), [codigoPaciente](const Paciente &p) {
        //aq no corpo vai ser o criterio da busca tipo coisa do sql msm
        return p.codigo == codigoPaciente;
        //basicamente um if tambem, só q dentro do return (if (p.codigo == codigoPaciente))
        //o valor do return vai ir pro "auto paciente", que vai ser o numero da posição no vetor
    });

    if (paciente == listaDePacientes.end()) {
        cout << "O paciente especificado nao foi encontrado." << endl;
        return; //return pra sair da função
    }

    cout << "Codigo do medico: ";
    getline(cin, input);
    cin.ignore(); //ignorar o \n do cin anterior
    if (!entradaEhNumero(input)) {
        cout << "Invalido: o codigo deve ser numerico." << endl;
        return;
    }
    codigoMedico = stoi(input);

    auto medico = find_if(listaDeMedicos.begin(), listaDeMedicos.end(), [codigoMedico](const Medico& m) {
        return m.codigo == codigoMedico;
    });

    if (medico == listaDeMedicos.end()) {
        cout << "O medico especificado nao foi encontrado." << endl;
        return;
    }

    do {
        cout << "Data da consulta (no formato dd/mm/aaaa): ";
        getline(cin, data);
        if (!validaData(data)) {
            cout << "Invalido: data nao esta no formato especificado." << endl;
        }
    } while (!validaData(data));

    do {
        cout << "Hora da consulta (no formato hh:mm): ";
        getline(cin, hora);
        if (!validaHora(hora)) {
            cout << "Invalido: hora nao esta no formato especificado." << endl;
        }
    } while (!validaHora(hora));

    //o if se acontece se a condição dentro for true
    //verificaDisponibilidade é false quando nao for disponivel
    //então !verificaDisponibilidade transforma em true, entrando no corpo do if
    //se for disponivel, verificaDisponibilidade retorna true, e o ! transforma em false, impedindo de entrar no if
    if (!verificaDisponibilidade(codigoMedico, data, hora)) {
        cout << "Medico indisponivel no horario especificado ou ja possui duas consultas no dia." << endl;
        return;
    }

    //se passar pelas verificações, vai criar a consulta e usar o push_back (da biblioteca <vector>) pra colocar no vetor listaDeConsultas
    //push_back adiciona como ultima posição
    //usando o construtor da classe
    Consulta consulta(codigo, data, hora, codigoMedico, codigoPaciente);
    listaDeConsultas.push_back(consulta);

    salvarConsulta(consulta);

    cout << "A consulta foi marcada com sucesso." << endl;
}

void cancelarConsulta() {
    int codigoConsulta;
    string input;
    cout << "Digite o codigo da consulta a ser cancelada: ";
    getline(cin, input);
    if (!entradaEhNumero(input)) {
        cout << "Invalido: o codigo deve ser numerico." << endl;
        return;
    }
    codigoConsulta = stoi(input);

    //mesma coisa do find_if do agendamento da consulta
    auto consulta = find_if(listaDeConsultas.begin(), listaDeConsultas.end(), [codigoConsulta](const Consulta& c) {
        return c.codigo == codigoConsulta;
    });

    //consulta seria = listaDeConsultas.end() se n encontrasse a consulta especificada
    //mas se n for ent encontrou e vai usar o erase do <vector>, q apaga um elemento do vetor
    if (consulta != listaDeConsultas.end()) {
        listaDeConsultas.erase(consulta);

        ifstream arquivoConsultas("consultas.txt");
        ofstream tempArquivo("temp_consultas.txt");
        string linha;

        while (getline(arquivoConsultas, linha)) {
            stringstream ss(linha);
            string codigo;
            getline(ss, codigo, ';');

            if (stoi(codigo) != codigoConsulta) {
                tempArquivo << linha << endl;
            }
        }

        arquivoConsultas.close();
        tempArquivo.close();

        remove("consultas.txt");
        rename("temp_consultas.txt", "consultas.txt");

        cout << "Consulta cancelada." << endl;
    } else {
        cout << "Consulta nao encontrada." << endl;
    }
}

void exibirConsultasPorData() {
    string data;
    cout << "Digite a data (no formato dd/mm/aaaa): ";
    //cin.ignore();
    getline(cin, data);

    cout << "Consultas na data " << data << ":" << endl;
    for (const auto &c : listaDeConsultas) {
        if (c.data == data) {
            cout << "------------" << endl
                 << "Codigo da consulta: " << c.codigo << endl
                 << "Hora: " << c.hora << endl
                 << "Codigo do Medico: " << c.codigoMedico << endl
                 << "Codigo do Paciente: " << c.codigoPaciente << endl
                 << "------------" << endl;
        }
    }
}

void exibirConsultasPorPaciente() {
    string input;
    int codigoPaciente;
    cout << "Digite o nome ou o codigo do paciente: ";
    //cin.ignore();
    getline(cin, input);

    //all_of da biblioteca <algorithm>
    //vai verificar se do começo ao fim do input é um digito com o ::isdigit
    //isdigit retorna um valor booleano
    bool verifCod = all_of(input.begin(), input.end(), ::isdigit);
    
    if (verifCod) {
        //stoi é da biblioteca <string>
        //ela transforma uma string em numeros int
        codigoPaciente = stoi(input);

        auto verificacao = find_if(listaDePacientes.begin(), listaDePacientes.end(), [codigoPaciente](const Paciente &p) {
            return p.codigo == codigoPaciente;
        });

        if (verificacao == listaDePacientes.end()) {
            cout << "O paciente especificado nao foi encontrado." << endl;
            return;
        }
    }
    else {
        codigoPaciente = 0;
        
        auto verificacao = find_if(listaDePacientes.begin(), listaDePacientes.end(), [input](const Paciente &p) {
            return p.nome == input;
        });

        if (verificacao == listaDePacientes.end()) {
            cout << "O paciente especificado nao foi encontrado." << endl;
            return;
        }
    }

    cout << "Consultas do paciente " << input << ":" << endl;
    for (const auto &c : listaDeConsultas) {
        for (const auto &p : listaDePacientes) {
            if ((verifCod && p.codigo == codigoPaciente) || (!verifCod && p.nome == input)) {
                if (c.codigoPaciente == p.codigo) {
                    cout << "------------" << endl
                         << "Codigo da consulta: " << c.codigo << endl
                         << "Data: " << c.data << endl
                         << "Hora: " << c.hora << endl
                         << "Codigo do medico: " << c.codigoMedico << endl
                         << "------------" << endl;
                }
            }
        }
    }
}

void exibirConsultasPorMedico() {
    string input;
    int codigoMedico;
    cout << "Digite o nome ou o codigo do medico: ";
    //cin.ignore();
    getline(cin, input);

    bool verifCod = all_of(input.begin(), input.end(), ::isdigit);
    
    if (verifCod) {
        //stoi é da biblioteca <string>
        //ela transforma uma string em numeros int
        codigoMedico = stoi(input);

        auto verificacao = find_if(listaDeMedicos.begin(), listaDeMedicos.end(), [codigoMedico](const Medico& m) {
            return m.codigo == codigoMedico;
        });

        if (verificacao == listaDeMedicos.end()) {
            cout << "O medico especificado nao foi encontrado." << endl;
            return;
        }
    }
    else {
        codigoMedico = 0;
        
        auto verificacao = find_if(listaDeMedicos.begin(), listaDeMedicos.end(), [input](const Medico& m) {
            return m.nome == input;
        });

        if (verificacao == listaDeMedicos.end()) {
            cout << "O medico especificado nao foi encontrado." << endl;
            return;
        }
    }

    cout << "Consultas do medico " << input << ":" << endl;
    for (const auto &c : listaDeConsultas) {
        for (const auto &m : listaDeMedicos) {
            if ((verifCod && m.codigo == codigoMedico) || (!verifCod && m.nome == input)) {
                if (c.codigoMedico == m.codigo) {
                    cout << "------------" << endl
                         << "Codigo da consulta: " << c.codigo << endl
                         << "Data: " << c.data << endl
                         << "Hora: " << c.hora << endl
                         << "Codigo do paciente: " << c.codigoPaciente << endl
                         << "------------" << endl;
                }
            }
        }
    }
}

void carregarDados() {
    ifstream arquivoPacientes("pacientes.txt");
    string linha;
    while (getline(arquivoPacientes, linha)) {
        stringstream ss(linha);
        string codigo, nome, endereco, telefone, dataDeNascimento;
        getline(ss, codigo, ';');
        getline(ss, nome, ';');
        getline(ss, endereco, ';');
        getline(ss, telefone, ';');
        getline(ss, dataDeNascimento, ';');
        //emplace_back, diferente do push_back, cria o objeto na hora, mas tem a mesma função de colocar no fim do vetor
        listaDePacientes.emplace_back(stoi(codigo), nome, endereco, telefone, dataDeNascimento);
    }
    arquivoPacientes.close();

    ifstream arquivoMedicos("medicos.txt");
    while (getline(arquivoMedicos, linha)) {
        stringstream ss(linha);
        string codigo, nome, telefone, especialidade;
        getline(ss, codigo, ';');
        getline(ss, nome, ';');
        getline(ss, telefone, ';');
        getline(ss, especialidade, ';');
        listaDeMedicos.emplace_back(stoi(codigo), nome, telefone, especialidade);
    }
    arquivoMedicos.close();

    ifstream arquivoConsultas("consultas.txt");
    while (getline(arquivoConsultas, linha)) {
        stringstream ss(linha);
        string codigo, data, hora, codigoMedico, codigoPaciente;
        getline(ss, codigo, ';');
        getline(ss, data, ';');
        getline(ss, hora, ';');
        getline(ss, codigoMedico, ';');
        getline(ss, codigoPaciente, ';');
        listaDeConsultas.emplace_back(stoi(codigo), data, hora, stoi(codigoMedico), stoi(codigoPaciente));
    }
    arquivoConsultas.close();
}

void menu() {
    carregarDados();
    int opcao;
    do {
        cout << "Viva Bem Menu:" << endl;
        cout << "1 - Cadastrar paciente" << endl;
        cout << "2 - Cadastrar medico" << endl;
        cout << "3 - Agendar consulta" << endl;
        cout << "4 - Cancelar consulta" << endl;
        cout << "5 - Exibir consultas por data" << endl;
        cout << "6 - Exibir consultas por paciente" << endl;
        cout << "7 - Exibir consultas por medico" << endl;
        cout << "8 - Sair" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;
        cin.ignore(); // Limpar buffer

        switch (opcao) {
            case 1:
                cadastroDoPaciente();
                break;
            case 2:
                cadastroDoMedico();
                break;
            case 3:
                agendamentoDaConsulta();
                break;
            case 4:
                cancelarConsulta();
                break;
            case 5:
                exibirConsultasPorData();
                break;
            case 6:
                exibirConsultasPorPaciente();
                break;
            case 7:
                exibirConsultasPorMedico();
                break;
            case 8:
                cout << "Saindo..." << endl;
                break;
            default:
                cout << "Opcao invalida." << endl;
        }
    } while (opcao != 8);
}

int main() {
    menu();
    return 0;
}