int led_verm = 13;                // Pino vermelho
int led_verde = 12;                // Pino verde
int led_azul = 27;
int led_amarel = 14;

int botao_verm = 33;              // Pino do botão vermelho
int botao_verde = 32;              // Pino do botão verde
int botao_azul = 18;
int botao_amarel = 19;

/* Flags para tratar debounce */
int flag_verm = 0;
int flag_verde = 0;
int flag_azul = 0;
int flag_amarel = 0;

const int MAX_RODADAS = 8;         // Número máximo de rodadas para vencer o jogo

int memoria_esp[MAX_RODADAS];      // Guarda a sequência de cores do jogo. O tamanho da memória é o número de máximo de rodadas
int memoria_jogador[MAX_RODADAS];  // Guarda a sequência de cores do jogador.

int modo = 0;                      // Modo do jogo: 0-Modo de Espera, 1-Modo de Jogo

int rodada = 1;                    // Indicador da rodada atual
int tempo_dificuldade = 350;       // Tempo entre as piscadas dos leds

void setup() {
  
  Serial.begin(9600); // Configura porta serial
  
  /* Configura portas dos leds e botões */
  pinMode(led_verm, OUTPUT);
  pinMode(led_verde, OUTPUT);
  pinMode(led_azul, OUTPUT);
  pinMode(led_amarel, OUTPUT);
  
  pinMode(botao_verm, INPUT_PULLUP);
  pinMode(botao_verde, INPUT_PULLUP);
  pinMode(botao_azul, INPUT_PULLUP);
  pinMode(botao_amarel, INPUT_PULLUP);  
}

void loop() {
  
  espera_inicio();
  
  pisca_leds(3);
  delay(1000);
  
  for(int rodada = 1; rodada <= MAX_RODADAS; rodada++) {

    turno_jogo(rodada);
    turno_jogador(rodada);

    if(!confere(rodada)) {
      Serial.println("Errou!");
      leds_derrota();
      break; 
    } else if(rodada == MAX_RODADAS){
      Serial.println("Venceu!");
      leds_vitoria();
    } else {
      liga_leds();
      delay(1000);
      desliga_leds();
    }

    delay(200);
  }  

  modo = 0;
  
}

void espera_inicio() {

  liga_leds();
  
  while( modo == 0 ){
    
    if( pressionou_verm() || pressionou_verde() || pressionou_azul() || pressionou_amarel()) {
      modo = 1;
    }
  }

  // Sorteia a sequência do jogo
  for(int i = 0; i < MAX_RODADAS; i++) {
    memoria_esp[i] = random(0,4);
  }
}

bool pressionou_verm() {

  if(digitalRead(botao_verm) == LOW && flag_verm == 0) { // Quando se aperta o botão
    flag_verm = 1;
    delay(7);
    return true;
  } else if(digitalRead(botao_verm) == HIGH && flag_verm == 1) { // Quando se solta o botão
    flag_verm = 0;
    delay(7);
    return false;
  }

  return false;
}

bool pressionou_verde() {

  if(digitalRead(botao_verde) == LOW && flag_verde == 0) { // Quando se aperta o botão
    flag_verde = 1;
    delay(7);
    return true;
  } else if(digitalRead(botao_verde) == HIGH && flag_verde == 1) { // Quando se solta o botão
    flag_verde = 0;
    delay(7);
    return false;
  }

  return false;
}
bool pressionou_azul() {

  if(digitalRead(botao_azul) == LOW && flag_azul == 0) { // Quando se aperta o botão
    flag_azul = 1;
    delay(7);
    return true;
  } else if(digitalRead(botao_azul) == HIGH && flag_azul == 1) { // Quando se solta o botão
    flag_azul = 0;
    delay(7);
    return false;
  }

  return false;
}

bool pressionou_amarel() {

  if(digitalRead(botao_amarel) == LOW && flag_amarel == 0) { // Quando se aperta o botão
    flag_amarel = 1;
    delay(7);
    return true;
  } else if(digitalRead(botao_amarel) == HIGH && flag_amarel == 1) { // Quando se solta o botão
    flag_amarel = 0;
    delay(7);
    return false;
  }

  return false;
}

void turno_jogo(int rodada) {

  Serial.println("Turno do Jogo: rodada " + (String) rodada);
  
  desliga_leds();
  
  for(int i = 0; i < rodada; i++) {
        
    if(memoria_esp[i] == 0) digitalWrite(led_verm, HIGH);
    if(memoria_esp[i] == 1) digitalWrite(led_verde, HIGH);
    if(memoria_esp[i] == 2) digitalWrite(led_azul, HIGH);
    if(memoria_esp[i] == 3) digitalWrite(led_amarel, HIGH);

    delay(tempo_dificuldade);
    
    desliga_leds();

    delay(tempo_dificuldade/2);
    
  }
}

void turno_jogador(int rodada) {

  Serial.println("Turno do Jogador: rodada " + (String) rodada);
  
  int leitura = 0;
  
  while(leitura < rodada) {
    
    desliga_leds();
    
    if(pressionou_verm()) {
      Serial.println("Vermelho");
      memoria_jogador[leitura] = 0;
      digitalWrite(led_verm, HIGH);
      delay(200);
      leitura++;
    }

    if(pressionou_verde()) {
      Serial.println("verde");
      memoria_jogador[leitura] = 1;
      digitalWrite(led_verde, HIGH);
      delay(200);
      leitura++;
    }
    if(pressionou_azul()) {
      Serial.println("Azul");
      memoria_jogador[leitura] = 2;
      digitalWrite(led_azul, HIGH);
      delay(200);
      leitura++;
    }
    if(pressionou_amarel()) {
      Serial.println("Amarelo");
      memoria_jogador[leitura] = 3;
      digitalWrite(led_amarel, HIGH);
      delay(200);
      leitura++;
    }
  }
  
  delay(1000);
  desliga_leds();
}

bool confere(int rodada) {
  
  for(int i = 0; i<rodada; i++) {
    if(memoria_esp[i] != memoria_jogador[i]) {
      return false;
    }
  }
  
  return true;
}

void liga_leds() {
  digitalWrite(led_verm, HIGH);
  digitalWrite(led_verde, HIGH);
  digitalWrite(led_azul, HIGH);
  digitalWrite(led_amarel, HIGH);
}

void desliga_leds() {
  digitalWrite(led_verm, LOW);
  digitalWrite(led_verde, LOW);
  digitalWrite(led_azul, LOW);
  digitalWrite(led_amarel, LOW);
}

void pisca_leds(int num) {
  for(int i = 0; i < num; i++) {
    liga_leds();
    delay(500);
    
    desliga_leds();
    delay(500); 
  }
}

void leds_derrota() {
  
  for(int i=0; i<15;i++) {
    liga_leds();
    delay(50);

    desliga_leds();
    delay(50); 
  }
}

void leds_vitoria() {
  
  desliga_leds();
  
  for(int i=0; i<15;i++) {
    if(i % 2 == 0) {
        digitalWrite(led_verm, HIGH);
        digitalWrite(led_verde, LOW);
        digitalWrite(led_amarel, HIGH);
        digitalWrite(led_azul, LOW);
        
    } else {
        digitalWrite(led_verm, LOW);
        digitalWrite(led_verde, HIGH);
        digitalWrite(led_amarel, LOW);
        digitalWrite(led_azul, HIGH);
    }
    delay(200);
  }

  desliga_leds();
}
