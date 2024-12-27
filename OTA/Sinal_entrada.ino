void Leitura_MUX()
{
  for(int m = 0; m <= 15; m++)
  {
    unsigned long acumulador = 0; // zera o acumulador para futura reutilização da variável
    
    for(int l = 0; l <= 3; l++) // LIGA AS PORTAS DIGITAIS RESPONSÁVEIS PELO CONTROLE DO MULTIPLEXADOR (ESCOLHE O SENSOR A SER LIDO) 
    {
      MUX_SAIDA.shiftWrite(PinosMUXEntrada[l], portaMUX_Entrada[m][l]);
    }

  
    for(int z = 9; z > 0; z--) // deslocamento para frente dos valores dentro do array. O décimo e último valor 'DadosSensores[9][z]' será deletado, por ser o mais antigo.
    {
      DadosSensores[m][z] = DadosSensores[m][z-1];
    }
    delay(100);
    
    //FILTRA_OSCILACAO();
    DadosSensores[m][0] = analogRead(A0);

    for(int p = 0; p <=9 ; p++)
    {
      acumulador = acumulador + DadosSensores[m][p];
    }

    Valor[m] = acumulador/10; // média
  }
}



void PrimeiraAquisicao() // ESSA FUNÇÃO PROPORCIONA AO PROGRAMA ENTRAR NA FUNÇÃO LOOP COM OS VALORES DOS SENSORES JÁ MEDIDOS
{
  for(int a = 0; a <= 15; a++)
  {
    for(int l = 0; l <= 3; l++) // LIGA AS PORTAS DIGITAIS RESPONSÁVEIS PELO CONTROLE DO MULTIPLEXADOR (ESCOLHE O SENSOR A SER LIDO) 
    {
      MUX_SAIDA.shiftWrite(PinosMUXEntrada[l], portaMUX_Entrada[a][l]);
    }
    delay(100);
    for(int b = 0; b <= 9; b++) // ARRAY COMPLETO PARA AQUISIÇÃO DE DADOS
    {
      DadosSensores[a][b] = analogRead(A0);
      delay(25);
    }
  }
}


/*
void FILTRA_OSCILACAO() // declarar m
{
  int p
  while (1)
  {
    p = analogRead(A0);
    if(p > 50)
    {
      DadosSensores[m][0] = p;
      break;
    }
    else
    {
      if(y < 50)
      {
        DadosSensores[m][0]  = p;
        break;
      }
    }
  }
}
*/
