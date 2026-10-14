# 🫓 Modelagem e Visualização 3D de um Parabolóide Hiperbólico


## 📝 Descrição


Este projeto foi desenvolvido em linguagem **C** como uma aplicação de modelagem e visualização computacional tridimensional, utilizando a biblioteca **raylib**.

O projeto tem como objetivo representar computacionalmente um **parabolóide hiperbólico**, uma superfície definida por uma função de duas variáveis, por meio da discretização de seu domínio e da construção de uma malha poligonal tridimensional.

O projeto também simula uma formiga em caminhada aleatória persistente sobre a superfície, mostrando sua posição, derivadas parciais, gradiente e vetor tangente.

O desenvolvimento busca estabelecer uma relação entre a modelagem computacional e conceitos estudados nas disciplinas de **Cálculo II** e **Geometria Analítica**.

---

## 📐 Modelo Matemático


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

A representação computacional é obtida por meio da discretização de um domínio elíptico, definido por:

$$
\frac{x^2}{1{,}5^2}+\frac{z^2}{1{,}0^2}\leq 1
$$

O domínio tem semieixo de 1,5 unidade na direção de $x$ e 1,0 unidade na direção de $z$. A malha é construída com anéis concêntricos de pontos, conectados por triângulos, de modo que a borda da superfície tenha formato elíptico.

---

## 🖥️ Visualização Computacional


A cena 3D permanece nítida; somente o corpo da formiga é desenhado em resolução 20% menor, com ampliação por pixels para um aspecto retrô. A trilha, as setas e os textos ficam nítidos. Os dados aparecem diretamente sobre o fundo, sem painel sólido. O visual também usa cores chapadas e saturadas, contraste escuro e formas facetadas.

As expressões e os valores matemáticos usam a fonte Cambria instalada no Windows; os títulos e controles usam Segoe UI.

A aplicação atualmente apresenta:

- 🟨 Superfície do parabolóide hiperbólico;
- 📐 Sistema de coordenadas tridimensional;
- 🧩 Malha poligonal composta por triângulos;
- 🖱️ Interação com a câmera;
- 🖥️ Redimensionamento da janela;
- ⛶ Alternância entre janela e tela cheia.

---

## 🐜 Simulação da Formiga


O estado da formiga é descrito pelas coordenadas horizontais $(x(t),z(t))$. A coordenada vertical é sempre recalculada pela superfície:

$$
y(t)=f(x(t),z(t))=0{,}2(x(t)^2-z(t)^2)
$$

A direção horizontal desejada muda em intervalos aleatórios. Para um ângulo de direção $\theta$, o vetor de movimento no domínio é $\mathbf{u}=(\cos\theta,\sin\theta)$, associando suas componentes a $x$ e $z$. A caminhada é persistente: a formiga mantém aproximadamente o rumo atual entre mudanças aleatórias de ângulo.

As derivadas parciais e o gradiente são:

$$
f_x=\frac{\partial f}{\partial x}=0{,}4x, \qquad
f_z=\frac{\partial f}{\partial z}=-0{,}4z, \qquad
\nabla f=(f_x,f_z)
$$

Pela regra da cadeia, a direção horizontal é levantada para um vetor tangente à superfície:

$$
\mathbf{T}=\left(u_x,\ f_xu_x+f_zu_z,\ u_z\right), \qquad
\widehat{\mathbf{T}}=\frac{\mathbf{T}}{\|\mathbf{T}\|}
$$

A formiga percorre a direção $\widehat{\mathbf{T}}$ com velocidade controlável. Ao se aproximar do limite elíptico, sua direção é refletida pela normal da borda para mantê-la no domínio. A trilha ciano mostra a trajetória parametrizada sobre a superfície. Setas destacam os vetores parciais $\mathbf{r}_x$ e $\mathbf{r}_z$, o gradiente, o vetor tangente à trajetória e a normal unitária. Os dados matemáticos aparecem diretamente sobre a cena.

Controles disponíveis:

- **Espaço:** pausar e continuar;
- **R:** reiniciar a caminhada usando a mesma semente aleatória;
- **N:** criar um novo percurso com outra semente;
- **Setas esquerda/direita:** diminuir ou aumentar a velocidade;
- **M:** alternar entre ocultar retas e caminho, mostrar apenas retas, apenas caminho ou ambos;
- **Scroll do mouse:** aproximar ou afastar a câmera.

O movimento aleatório não segue o gradiente; o gradiente é exibido para permitir a comparação entre o rumo escolhido e a direção de maior crescimento da função. A simulação explora conceitos de **Cálculo II**, incluindo:

- Derivadas parciais;
- Vetor gradiente;
- Vetores tangentes à superfície;
- Vetor normal;
- Parametrização de curvas;
- Trajetórias sobre superfícies;
- Relação entre a função e sua geometria.

Uma etapa futura poderá acrescentar um modo em que a formiga siga o gradiente ou seu oposto.

---

## 🧮 Conceitos Matemáticos


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


```text
projeto-paraboloide/

├── main.c          # Programa principal e gerenciamento da visualização
├── formiga.c       # Movimento e desenho da formiga sobre a superfície
├── formiga.h       # Estrutura e funções da formiga
├── pringles.c      # Função matemática e construção da malha
├── pringles.h      # Declarações das funções do módulo
└── executar.bat    # Compila e executa no Windows com MSYS2 UCRT64
