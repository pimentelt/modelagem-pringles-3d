# 🥔 Modelagem e Visualização 3D de um Parabolóide Hiperbólico

[svg](https://github.com/pimentelt/projeto-paraboloide#-modelagem-e-visualização-3d-de-um-parabolóide-hiperbólico)

## 📝 Descrição

[svg](https://github.com/pimentelt/projeto-paraboloide#-descrição)

Este projeto foi desenvolvido em linguagem **C** como uma aplicação de modelagem e visualização computacional tridimensional, utilizando a biblioteca **raylib**.

O projeto tem como objetivo representar computacionalmente um **parabolóide hiperbólico**, uma superfície definida por uma função de duas variáveis, por meio da discretização de seu domínio e da construção de uma malha poligonal tridimensional.

Como parte da evolução do projeto, será implementada futuramente a **simulação do movimento de uma formiga sobre a superfície**, utilizando conceitos matemáticos relacionados à descrição de trajetórias, vetores, derivadas parciais e vetor gradiente.

O desenvolvimento busca estabelecer uma relação entre a modelagem computacional e conceitos estudados nas disciplinas de **Cálculo II** e **Geometria Analítica**.

---

## 📐 Modelo Matemático

[svg](https://github.com/pimentelt/projeto-paraboloide#-modelo-matemático)

A superfície é definida pela função de duas variáveis:

$$
f(x,z) = a(x^2-z^2)
$$

No modelo atual, é utilizado:

$$
a = 0,2
$$

Assim, a coordenada vertical de cada ponto da superfície é determinada por:

$$
y = 0,2(x^2-z^2)
$$

O par $(x,z)$ determina a posição do ponto no domínio horizontal, enquanto o valor $y=f(x,z)$ determina sua altura.

A representação computacional é obtida por meio da discretização do domínio em uma grade de pontos. Os pontos adjacentes são conectados por triângulos, formando uma **malha poligonal** que aproxima a superfície matemática contínua.

---

## 🖥️ Visualização Computacional

[svg](https://github.com/pimentelt/projeto-paraboloide#-visualização-computacional)

A superfície tridimensional é renderizada utilizando a biblioteca **raylib**, permitindo sua visualização em um ambiente interativo.

A aplicação atualmente apresenta:

- 🟨 Superfície do parabolóide hiperbólico;
- 📐 Sistema de coordenadas tridimensional;
- 🧩 Malha poligonal composta por triângulos;
- 🖱️ Interação com a câmera;
- 🖥️ Redimensionamento da janela;
- ⛶ Alternância entre janela e tela cheia.

---

## 🐜 Simulação da Formiga

[svg](https://github.com/pimentelt/projeto-paraboloide#-simulação-da-formiga)

Como etapa futura do projeto, será implementada uma formiga capaz de se deslocar sobre a superfície modelada.

A trajetória será utilizada como elemento para explorar computacionalmente conceitos de **Cálculo II**, incluindo:

- Derivadas parciais;
- Vetor gradiente;
- Vetores tangentes à superfície;
- Vetor normal;
- Parametrização de curvas;
- Trajetórias sobre superfícies;
- Relação entre a função e sua geometria.

A intenção é que a posição da formiga seja determinada matematicamente pela superfície, evitando que seu movimento seja apenas uma animação independente da função.

---

## 🧮 Conceitos Matemáticos

[svg](https://github.com/pimentelt/projeto-paraboloide#-conceitos-matemáticos)

O projeto utiliza e pretende explorar os seguintes conceitos:

- **Funções de duas variáveis**
- **Domínio e imagem**
- **Superfícies no espaço tridimensional**
- **Discretização de funções**
- **Derivadas parciais**
- **Vetor gradiente**
- **Vetores tangentes**
- **Vetores normais**
- **Curvas parametrizadas**
- **Representação geométrica de funções**

---

## 💻 Linguagem, Biblioteca e Dependências

[svg](https://github.com/pimentelt/projeto-paraboloide#-linguagem-biblioteca-e-dependências)

- **Linguagem:** C
- **Padrão:** C11
- **Biblioteca gráfica:** raylib
- **Bibliotecas utilizadas:** `raylib.h`, `rlgl.h`, `stdlib.h`, `math.h`
- **Plataforma principal:** Linux
- **Compilador:** GCC
- **Sistema de construção:** CMake / GCC
- **Renderização:** OpenGL através da infraestrutura gráfica do raylib

---

## 📁 Estrutura do Repositório

[svg](https://github.com/pimentelt/projeto-paraboloide#-estrutura-do-repositório)

```text
projeto-paraboloide/

├── main.c          # Programa principal e gerenciamento da visualização
├── pringles.c      # Função matemática e construção da malha
└── pringles.h      # Declarações das funções do módulo
