#include "packet2.h"

packet::packet(){
    packet_count = 0;
}

packet::~packet(){

}

void packet::getServerPacket(u8 *pack){
    for (int i = 0; i < packet_count; ++i){
        pack[i] = serverPacket[i];
    }
}

void packet::setServerPacket(u8 *pack){
    for (int i = 0; i < packet_count; ++i)
        serverPacket[i] = pack[i];
}

int packet::getPacketCount(){
    return packet_count;
}

// Limpa os pacotes
void packet::clearPacket(){
    memset(serverPacket, 0x0, 50);
    packet_count = 0;
}

// Cabeça do Pacote 1
void packet::writeHeader_1(u32 HEADER){
    memcpy(serverPacket, &HEADER, sizeof(HEADER));
    packet_count += 4;
}

// Cabeça do Pacote 2
void packet::writeHeader_2(u32 HEADER){
    memcpy(serverPacket + 4, &HEADER, sizeof(HEADER));
    packet_count += 4;
}

// Escreve um byte
void packet::writeByte(u8 b){
    serverPacket[packet_count] = b;
    packet_count++;
}

// Escreve 2 bytes
void packet::writeShort(u16 s){
    memcpy(serverPacket + packet_count, &s, 2);
    packet_count += 2;
}

// Escreve 4 bytes
void packet::writeInt(u32 i){
    memcpy(serverPacket + packet_count, &i, 4);
    packet_count += 4;
}


void packet::writeChecksum(){

}

// Ler 4 bytes
u32 packet::readInt(){
    u32 packet = serverPacket[packet_count];
    packet_count++;
    packet = ((serverPacket[packet_count] << 8)  | packet);
    packet_count++;
    packet = ((serverPacket[packet_count] << 16) | packet);
    packet_count++;
    packet = ((serverPacket[packet_count] << 24) | packet);
    packet_count++;
    return packet;
}

// Ler 2 bytes
u16 packet::readShort(){
    u16 packet = serverPacket[packet_count];
    packet_count++;
    packet = ((serverPacket[packet_count] << 8)  | packet);
    packet_count++;
    return packet;
}

// Ler 1 byte
u8  packet::readByte(){
    u8 packet = serverPacket[packet_count];
    packet_count++;
    return packet;
}

/* Pacotes de Comunicação entre Cliente/Servidor */
// Pacote para fazer Login
void packet::packet_Login(){

    writeInt(CLIENT_PACKET_UNK); // Pacote desconhecido, pode ser um head
    writeInt(CLIENT_PACKET_LOGIN_SEND); // Provavelmente o header para Login
    writeShort(0xBB0D);
    writeByte(0x0D);
    writeInt(0x69676964);   //SENHA
    writeInt(0x326C6174);   //SENHA
    writeByte(0x31);        //SENHA
    writeByte(0x0D);    
    writeShort(0x300A); // Desconhecido????????
}

/* Analisa cada pacote e sua estrutura para saber como 
tratar cada um */
void packet::packetHandle(){
    u32 HEADER_1 = readInt();
    u32 HEADER_2 = readInt();

    switch (HEADER_1) {
        

        default:
            printf("Packet Invalido ou nao implementado: 0x%X\n", HEADER_1);
        break;
    }
}

void packet::setTemp(u8 temp[3]){
    /*  ON = 0x4E4F;
        OFF = 0x4F4646;
    */

    clearPacket();
    writeInt(CLIENT_PACKET_UNK);
    writeByte(0x34);    //Comando para: Ligar, desligar, alterar temperatura
    writeByte(0x00);    //Padrão
    writeShort(0x0000); //Padrão
    writeShort(0xee49); //CHECKSUM == VARIÁVEL!!!!!!!!!!!!
    /* ........ */
    writeShort(0x2720);
    writeInt(0x3D207265);
    writeInt(0x706F2074);
    writeInt(0x65732063);
    writeInt(0x61206574);
    writeInt(0x61647075);
    /*escrever: 75706461 74652061 63207365 74206f70 6572203d 2027  só que ao contrário =]*/
    /*escrever: 272c6d6f 6465203d 2027434f 4f4c272c 66616e20 3d202741 55544f27 2c737769 6e67203d 
    20274f46 46272c6c 6f636b20 203d2027 4f464627 2c736574 74656d70 20203d20 27*/
    /*ESCREVER TEMPERATURA*/
    /*escrever int*/
    /*escrever . = 2e*/
    /*escrever float  || 0 = 30*/
    /*end temp*/
    /*escrever: 272c7365 7474656d 706c6f63 6b203d20 274f4646 272c6d6f 64656c6f 636b203d 2
   0274f46 46272c73 65747465 6d705f6c 6c696d20 3d202731 36272c73 65747465 6d705f75 6c696d20 3 
   d202733 30272c65 78655f63 6e74203d 2031302c 20657865 203d2027 4f4e272c 20616666 65637420 3 
   d202757 45422720 77686572 65206164 64722069 6e202836 36293b00 2f*/
    /*Determinar comando on/ off*/
    if (command == 1)
        writeShort(0x4E4F);
    else{
        writeShort(0x4646);
        writeByte(0x4F);
    }

 }

 void packet::setCommand(int value){
    command = value //ON OU OFF
 }
