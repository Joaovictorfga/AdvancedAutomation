#include <iostream>
#include <queue>
using namespace std;

class Item {

private:

    int length; //cm
    int width;  //cm
    int height; //cm
    long int volume; //cm³
    int weight; //gm
    int destinyID;
    bool isPackage; //True => Pacote | False => Envelope

public:

    Item(int destinyID, bool isPackage) { //CONSTRUTOR 1: Caixa e Envelope padrão (Pacote => 16x11x6x500 | Envelope => 11x16x2x200)
        this->length = 16;
        this->width = 11;
        this->destinyID = destinyID;
        this->isPackage = isPackage;
        if (isPackage) {
            this->height = 6;
            this->weight = 500;
            this->volume = 16 * 11 * 6;
        }
        else {
            this->height = 2;
            this->weight = 200;
            this->volume = 16 * 11 * 2;
        }
    }

    Item(int length, int width, int height, int weight, int destinyID, bool isPackage) { //CONSTRUTOR 2: Caixa com tamanho variável
        if (isPackage) {
            this->length = length;
            this->width = width;
            this->height = height;
            this->weight = weight;
            this->destinyID = destinyID;
            this->isPackage = isPackage;
            this->volume = length * width * height;
        }
        else {
            Item(destinyID, isPackage);
        }
    }

    int getDestiny() {
        return this->destinyID;
    }

    bool isaPackage() {
        return this->isPackage;
    }
};


class Storage { //Lugar que armazena os pacotes até que eles sejam recolhidos

private:

    bool isWorking = true;
    int LIMIT = 5;
    queue <Item> item_queue;

public:

    void addItem(Item x) { //Um retorno FALSE indica que o armazem atingiu seu limite
        item_queue.push(x);
        cout << "\nItem movido da fila 2 para armazem\n";
        cout << "\nQuantidade no Armazem: " << item_queue.size() << endl;
        if (item_queue.size() == LIMIT) {
            this->isWorking = false;
        }
    }

    bool isAviable() {
        return this->isWorking;
    }

    void clearStorage() {
        while (!item_queue.empty())
        {
            item_queue.pop();
        }
    }
};


class Track2 { //Esteira que separa os pacotes dos envelopes

private:

    bool isWorking;
    int destinyID;
    queue <Item> item_queue;
    Storage* s[2];

public:

    Track2(int destinyID) {
        this->isWorking = true;
        this->destinyID = destinyID;
        for (int i = 0; i < 2; i++) {
            s[i] = new Storage();
        }
    }

    void separateEnvAndPack() { //Um retorno FALSE indica que o armazem atingiu seu limite
        if (!this->item_queue.empty()) {
            Item x = item_queue.front();
            if (x.isaPackage()) { //É um pacote?
                if (s[0]->isAviable()) { //Armazem dos pacotes está disponível?
                    s[0]->addItem(x); //Se estiver, adicione o item 
                    item_queue.pop();
                    this->isWorking = true;
                }
                else {
                    this->isWorking = false;
                }
            }
            else { //Se não é um pacote, é um envelope
                if (s[1]->isAviable()) { //Armazem dos envelopes está disponível?
                    s[1]->addItem(x); //Se estiver, adicione o item 
                    item_queue.pop();
                    this->isWorking = true;
                }
                else {
                    this->isWorking = false;
                }
            }
        }
    }

    bool addItem(Item x) { //Um retorno FALSE indica que a esteira está travada
        if (this->isWorking) {
            item_queue.push(x);
            cout << "\nItem movido para a fila 2. Tamanho da fila 2: " << item_queue.size() << endl << endl;
            return true;
        }
        return false;
    }

};


class Track1 { //Esteira que separa os destinos dos itens

private:

    bool isWorking;
    Track2* tracks[3];
    queue <Item> item_queue;

    void separateDestiny() { //Leva o item até a sua esteira correspondente (DESTINO NÃO ENCONTRADO AINDA NÃO FOI IMPLEMENTADO)
        if (!this->item_queue.empty()) { //Se a fila não está vazia
            if (this->isWorking) {
                Item x = this->item_queue.front(); //Seleciona o primeiro elemento da fila
                this->item_queue.pop();
                int destinyID = x.getDestiny(); //Observa-se o destino do elemento
                if (!tracks[destinyID]->addItem(x)) { //Tenta adicionar à esteira. Se ela estiver travada, o objeto é enviado novamente ao inicio dessa fila
                    this->item_queue.push(x);
                }
            }
        }
    }

public:

    Track1() {
        this->isWorking = true;
        for (int i = 0; i < 3; i++) {
            tracks[i] = new Track2(i);
        }
    }

    void addItem(Item x) {
        this->item_queue.push(x);
        cout << "\nItem adicionado! Tamanho da fila 1: " << item_queue.size() << endl << endl;
        cout << "---------------------------------------\n\n";
    }

    void run() {
        for (int i = 0; i < 3; i++) {
            tracks[i]->separateEnvAndPack(); //THREAD 2, 3 e 4
        }
        separateDestiny(); //THREAD 1
        cout << "Tamanho da fila 1: " << item_queue.size() << endl << endl;
        cout << "---------------------------------------\n\n";
    }
};


int main()
{
    Track1* t = new Track1();
    Item* x = new Item(0, true);
    string option;
    while (true) {
        cout << "1. Adicionar objeto na fila\n";
        cout << "2. Correr Esteira\n\n";
        cin >> option;
        if (option == "1") {
            t->addItem(*x);
        }
        if (option == "2") {
            t->run();
        }
    }
}

