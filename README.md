# Embedded-Systems-Project (English)

Digital Safe Box

## About the project

This project objective was to make a system that had similar behavior to a safe box lock mechanism. The system was developed with an embedded computer, instead of the mechanical mechanism that is usually used. The embedded computer used was a Beagle Bone Black. 

![](GifdoProjeto.gif)

## Components - Bill of Materials

* Beaglebone Black Rev C, with Debian 9.5 2018-10-07 4GB SD IoT installled;
* 1 LED
* 1 4 bit digital encoder
* 4 3.3K Resistors 
* 5 1K Resistors
* 1 Protype board
* Wires

## Project Schematic

![alt text](https://docs.google.com/drawings/d/e/2PACX-1vQb0JCP48ShjpVStbLx8hNEBLTI6bsRhMRw4brJcJUAHbIoU5h5Pbxlj3mSAH578EQZy6Aa61WzGuFH/pub?w=480&h=360)


### History of the project development 

The main requisite of this project was to make something simple since 3 of the 4 members of the group were doing an internship outside São Carlos. With that in mind, the initial idea was to make a voice-activated lock, but then we found that the voice recognition algorithms would be more complicated than we initially thought and it would require work outside the scope of the course. The group then decided to make a digital lock using an absolute encoder, since this project would use only digital inputs/outputs.

For this project, we used the C language to code the software. We have done that for two reasons. First, It was implied that one of the objectives of this course was to teach the use of Linux on an embedded computer. The use of other high-level languages, such as Python and javascript would hide the implementation details and this course objective would not be attained. The second reason is that one of the members of the group has previous experience with bonescript and he knew that the language does not have a fast enough response time for this project.

The code for controlling digital outputs was straightforward, most of the difficulties found were in reading the inputs in real-time. These problems were solved by implementing a debouncing function and also by using a GPIO reading function(source code can be found in a link bellow).

The debouncing code used:

```
void DebounceSwitch(int *Key_changed,int *status,int *previous_status, int pin, int *Count)
{
    int RawState=0;
    *Key_changed = 0;
    gpio_get_value(pin,&RawState);
    if (RawState == *status) {
        // Set the timer which will allow a change from the current state.
        if (*status==0) *Count = 5;
        else                 *Count = 2;
    } else {
        // Key has changed - wait for new state to become stable.
        if (--*Count == 0) {
            // Timer expired - accept the change.
            *previous_status = *status;
            *status = RawState;
            *Key_changed=1;
            // And reset the timer.
            if (*status==0) *Count = 5;
            else                 *Count = 2;
        }
    }
}
```

## Libraries Used
* [GPIO functions](https://github.com/aferodeveloper/afLib/blob/master/linux/gpiolib.cpp) - We used these functions to read the inptus form the Beagle Bone Black

## Project Video
  [Project Video](https://github.com/Scheffer888/Alohomora/blob/master/V%C3%ADdeo%20do%20Projeto) 
## Project team

* Eduardo Belisario Scheffer 
* Rodrigo Lorey 
* Rodrigo Monteiro Consoni Bonaccorsi 
* Victor Takao Bernardino Shime 


# Projeto de Sistemas Embarcados (Portugês)

Cofre Digital

## Sobre o projeto

O objetivo desse projeto era criar um sistema com comportamento semelhante ao um mecanismo de trava de um cofre. O sistema foi feito utilizando um computador embarcado ao invés do mecanismo mecânico normalmente utilizado. O computador embarcado utilizado foi a Beagle Bone Black.

![](GifdoProjeto.gif)

## Componentes Lista de Materiais

* Beaglebone Black Rev C, com Debian 9.5 2018-10-07 4GB SD IoT instalado;
* 1 LED
* 1 4 bit encoder digital
* 4 3.3K Resistores 
* 5 1K Resistores
* 1 placa de prototipagem
* Fios

## Esquemático do projeto

![alt text](https://docs.google.com/drawings/d/e/2PACX-1vQb0JCP48ShjpVStbLx8hNEBLTI6bsRhMRw4brJcJUAHbIoU5h5Pbxlj3mSAH578EQZy6Aa61WzGuFH/pub?w=480&h=360)
![alt text](https://docs.google.com/drawings/d/e/2PACX-1vSLnZQH8X6IR1c-Td4bJW6P8SZgz0G004u8mvSXAZOpX-wgTBJ60BAs1oOIyZVnBQzvBHFo1dm0Z1zh/pub?w=480&h=360)

### História do desenvolvimento do projeto

O principal requisto do projeto era fazer algo simples, pois três dos 4 membros do grupo estão fazendo estágio fora de São Carlos. Com esse requisito em mente, a ideia inicial era fazer um trava desbloqueada por voz, porém nós descobrimos que o algoritimo de reconhecimento de voz seria mais complicado do que incialmente estimado e iria necessitar de trabalho além do escopo da disciplina. O grupo então decidiu por fazer uma trava eletrôncia utlizando um encoder digital, pois esse projeto iria necessitar apenas de inputs e outputs digitais. 

Nesse projeto foi utlizado a linguagem  C para criar o código. Isso foi feito por dois motivos. Primeiro, foi assumido que um dos objetivos da disciplina era ensinar a utlizar Linux em sistemas embarcados. O uso de linguagens de alto nivel como Python ou JavaScirpty iria obscurecer os detalhes de implementação e esse objetivo não seria alcançado. A segunda razão é que um dos membros do grupo já possui experiencia com BoneScript e sabia que a linghagem não teria um tempo de resposta adequado.

O código para controlar os outputs não teve muitas dificuldades durante o seu desenvolvimento, a maior dificutldade foi  em criar um código ler os inputs. Esse problema foi resovido implementando uma função de "debouncing" e usando uma função de GPIO(código fonte pode ser encontrado abaixo)

O código de "debouncing" utlizado:

```
void DebounceSwitch(int *Key_changed,int *status,int *previous_status, int pin, int *Count)
{
    int RawState=0;
    *Key_changed = 0;
    gpio_get_value(pin,&RawState);
    if (RawState == *status) {
        // Set the timer which will allow a change from the current state.
        if (*status==0) *Count = 5;
        else                 *Count = 2;
    } else {
        // Key has changed - wait for new state to become stable.
        if (--*Count == 0) {
            // Timer expired - accept the change.
            *previous_status = *status;
            *status = RawState;
            *Key_changed=1;
            // And reset the timer.
            if (*status==0) *Count = 5;
            else                 *Count = 2;
        }
    }
}
```

## Bibliotecas Utlizadas
* [Funções de GPIO ](https://github.com/aferodeveloper/afLib/blob/master/linux/gpiolib.cpp) - Usamos uma dessa funções para ler os inputs

## Video do Projeto
  [Video do Projeto](https://github.com/Scheffer888/Alohomora/blob/master/V%C3%ADdeo%20do%20Projeto) 

## Membros da equipe

* Eduardo Belisario Scheffer 
* Rodrigo Lorey 
* Rodrigo Monteiro Consoni Bonaccorsi 
* Victor Takao Bernardino Shime 



