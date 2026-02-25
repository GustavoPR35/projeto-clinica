
# Sistema de Agendamento "Viva Bem"

Descrição
- Programa em C++ para cadastro de pacientes e médicos, agendamento e cancelamento de consultas, e exibição de consultas por data, paciente ou médico.

Requisitos
- Compilador C++ compatível com C++11 ou superior (ex.: g++ do MinGW, MSVC).

Arquivos gerados/usuais
- `pacientes.txt`: registros de pacientes, formato por linha: `codigo;nome;endereco;telefone;dataDeNascimento`
- `medicos.txt`: registros de médicos, formato por linha: `codigo;nome;telefone;especialidade`
- `consultas.txt`: registros de consultas, formato por linha: `codigo;data;hora;codigoMedico;codigoPaciente`
- `ultimo_codigo_paciente.txt`, `ultimo_codigo_medico.txt`, `ultimo_codigo_consulta.txt`: controlam os códigos sequenciais gerados.

Como compilar (MinGW / g++)
```
g++ -std=c++11 -O2 -o clinica clinica.cpp
```

Como compilar (Visual Studio - Developer Command Prompt)
```
cl /EHsc /std:c++14 clinica.cpp /Fe:clinica.exe
```

Como executar
- No Windows, no mesmo diretório do executável:
```
.\clinica.exe
```
ou
```
.\clinica
```

Uso básico
- Ao executar, o programa apresenta um menu com opções:
  - `1` - Cadastrar paciente
  - `2` - Cadastrar médico
  - `3` - Agendar consulta
  - `4` - Cancelar consulta
  - `5` - Exibir consultas por data
  - `6` - Exibir consultas por paciente
  - `7` - Exibir consultas por médico
  - `8` - Sair

Validações e formatos
- Data: `dd/mm/aaaa` (ex.: `31/12/1990`).
- Hora: `hh:mm` (ex.: `14:30`, 24h).
- Códigos: entradas numéricas quando solicitado (somente dígitos).
- Regras de agendamento: um médico pode ter no máximo 2 consultas no mesmo dia e consultas não podem ter intervalo menor que 30 minutos entre si.

Observações
- Se os arquivos de dados não existirem, o programa os criará ao salvar registros ou ao inicializar os códigos.
- Para reiniciar as sequências de códigos, remova (ou edite) os arquivos `ultimo_codigo_*.txt` — cuidado: isso poderá causar colisão de códigos com registros existentes.
- Os arquivos de dados são de texto simples; você pode abri-los com qualquer editor para inspeção/backup.

Exemplo rápido
1. Compile o programa.
2. Execute o binário.
3. Escolha `1` e cadastre um paciente.
4. Escolha `2` e cadastre um médico.
5. Escolha `3` para agendar uma consulta usando os códigos exibidos.
