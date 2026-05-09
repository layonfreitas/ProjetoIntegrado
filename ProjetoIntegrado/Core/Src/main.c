/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7735\st7735.h"
#include <stdlib.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BotaoLe  HAL_GPIO_ReadPin(BOTAO9_GPIO_Port, BOTAO9_Pin)
#define BotaoLe1 HAL_GPIO_ReadPin(BOTAO10_GPIO_Port, BOTAO10_Pin)
#define BotaoLe2 HAL_GPIO_ReadPin(BOTAO11_GPIO_Port, BOTAO11_Pin)
#define BotaoLe3 HAL_GPIO_ReadPin(BOTAO12_GPIO_Port, BOTAO12_Pin)
#define limpar ST7735_FillScreen(BLACK)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
int alunosMax = 0, alunos = 0;
int matricula = 1000;
int totalEntradas = 0, totalSaidas = 0, totalRetornos = 0;
char AlunosFora[4] = "0" , AlunosDentro[4] =  "0", AlunosMax[4] =  "0";
uint32_t inicioAula;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void inicio(void);
void digitarsenha(void);
void configurarAlunos(void);
void menu(void);
void entrada(void);
void relatorio(void);
void barra(int dentro, int maximo);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	  bool iniciou = false;
	  bool senhaGerada = false;
	  bool digitandoSenha = false;
	  bool senhaCorreta = false;
	  int senha, cliques = 0, digitado = 0, digitos = 0, tentativas = 0;
	  uint32_t ultimoClique = 0;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  ST7735_Init();
  ST7735_InvertColors(false);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  if (iniciou == false)
	  {
		  limpar;
		  inicio();
		  iniciou = true;
	  }

	  if (BotaoLe == 0 && senhaGerada == false)
	      {
		  limpar;
		  digitarsenha();
		  digitandoSenha = true;
		  srand(HAL_GetTick());
		  senha = (rand() % 900) + 100;
		  senhaGerada = true;
	      }

	  if (BotaoLe1 == 0 && digitandoSenha == true){
		  char *digitoTexto = "";
		  cliques++;
		  ultimoClique = HAL_GetTick();

		  HAL_Delay(200);
		  while(BotaoLe1 == 0)
		  {

		  }
		  switch(cliques){
		      case 1:
		          digitoTexto = "1";
		          break;
		      case 2:
		          digitoTexto = "2";
		          break;
		      case 3:
		          digitoTexto = "3";
		          break;
		      case 4:
		          digitoTexto = "4";
		          break;
		      case 5:
		          digitoTexto = "5";
		          break;
		      case 6:
		          digitoTexto = "6";
		          break;
		      case 7:
		          digitoTexto = "7";
		          break;
		      case 8:
		          digitoTexto = "8";
		          break;
		      case 9:
		          digitoTexto = "9";
		          break;
		      case 10 :
		    	  digitoTexto = "0";
		    	  break;
		      default:
		          cliques = 0;
		          break;
		  }

		  ST7735_WriteString(digitos * 10, 20, digitoTexto, Font_7x10, WHITE, BLACK);

		  }


	  if (cliques > 0 && (HAL_GetTick() - ultimoClique) > 1000){
		  digitado = digitado * 10 + cliques;
		  digitos++;

		  cliques = 0;

		  if (digitos == 3)
		  {
			  limpar;
			  if (digitado == senha)
			  {
				  ST7735_WriteString(0,0,"ACESSO LIBERADO", Font_16x26, GREEN, BLACK);
				  senhaCorreta = true;
				  digitandoSenha = false;
				  HAL_Delay(500);

			  }
			  else
			  {
				  tentativas ++;
				  if(tentativas == 3){
					  ST7735_WriteString(0,0,"Voce errou a senha varias vezes, espere 30 segundos", Font_11x18, RED, BLACK);
					  HAL_Delay(30000);
					  ST7735_WriteString(0,0,"SENHA INCORRETA, TENTE NOVAMENTE", Font_11x18, RED, BLACK);
					  tentativas = 0;
					  digitado = 0;
				      digitos = 0;
					  limpar;
				  }
				  else {
				  ST7735_WriteString(0,0,"SENHA INCORRETA, TENTE NOVAMENTE", Font_11x18, RED, BLACK);
				  digitado = 0;
				  digitos = 0;
				  }


			  }
		  }

		  if (senhaCorreta == true)
		  {
			  configurarAlunos();
			  menu();


			  senhaCorreta = false;
			  senhaGerada = false;
			  digitandoSenha = false;
			  digitado = 0;
			  digitos = 0;
			  cliques = 0;
			  iniciou = false;
			  alunosMax = 0;
			  alunos = 0;
			  matricula = 1000;
			  totalEntradas = 0;
			  totalSaidas = 0;
			  totalRetornos = 0;

		  }




}



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ST7735_DC_Pin|ST7735_RES_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ST7735_CS_Pin */
  GPIO_InitStruct.Pin = ST7735_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ST7735_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ST7735_DC_Pin ST7735_RES_Pin */
  GPIO_InitStruct.Pin = ST7735_DC_Pin|ST7735_RES_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : BOTAO9_Pin BOTAO10_Pin BOTAO11_Pin BOTAO12_Pin */
  GPIO_InitStruct.Pin = BOTAO9_Pin|BOTAO10_Pin|BOTAO11_Pin|BOTAO12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
 void inicio(void)
 {
		ST7735_WriteString(0,0,"Pressione o botao para o comeco da aula ", Font_7x10, WHITE, BLACK);

 }

 void digitarsenha(void)
 {

	 ST7735_WriteString(0,0,"Bem vindo, Digite a sua senha: ", Font_7x10, WHITE, BLACK);
 }


 void configurarAlunos(void)
 {
	 bool configurandoAlunos =false;
	 char textoAlunos[20];

	 limpar;
	 ST7735_WriteString(0,0,"Digite a quantidade de alunos ", Font_7x10, WHITE, BLACK);

	 while(configurandoAlunos == false){

		 if (BotaoLe1 == 0)
		 {
			 alunosMax++;
			 HAL_Delay(200);


		 }

		 else if (BotaoLe2 == 0 && alunosMax > 0)
		 {
			 alunosMax--;
			 HAL_Delay(200);
		 }

		 else if (BotaoLe == 0 && alunosMax > 0)
		 {

			 configurandoAlunos = true;
			 HAL_Delay(200);
		 }

		 sprintf(textoAlunos, "%-2d", alunosMax);
		 ST7735_WriteString(0,20,"Alunos: ", Font_7x10, WHITE, BLACK);
		 ST7735_WriteString(57,20,textoAlunos, Font_7x10, WHITE, BLACK);

	 }

	 limpar;
	 ST7735_WriteString(0,0,"O numero max de alunos: ", Font_7x10, WHITE, BLACK);
	 ST7735_WriteString(14,10,textoAlunos, Font_7x10, WHITE, BLACK);
	 HAL_Delay(500);


 }
 void menu(void)
 {
	 limpar;
	 int alunosDentro = 0;
	 int alunosFora = 0;
	 sprintf(AlunosMax, "%-2d", alunosMax);
	 sprintf(AlunosDentro, "%-2d", alunosDentro);
	 sprintf(AlunosFora, "%-2d", alunosFora);

	 inicioAula = HAL_GetTick();

	 ST7735_WriteString(0,0,"Max de alunos: ", Font_7x10, WHITE, BLACK);
	 ST7735_WriteString(99,0, AlunosMax, Font_7x10, WHITE, BLACK);
	 ST7735_WriteString(0,10,"Alunos na sala: ", Font_7x10, WHITE, BLACK);
	 ST7735_WriteString(107,10,AlunosDentro, Font_7x10, WHITE, BLACK);
	 ST7735_WriteString(0,20,"Alunos fora: ", Font_7x10, WHITE, BLACK);
	 ST7735_WriteString(86,20, AlunosFora, Font_7x10, WHITE, BLACK);


	 while(true){
		 if(BotaoLe == 0)
		 {
			 entrada();
			 alunosDentro = alunos;
			 sprintf(AlunosDentro, "%-2d", alunosDentro);
			 HAL_Delay(200);
			 ST7735_WriteString(107,10,AlunosDentro, Font_7x10, WHITE, BLACK);
			 while(BotaoLe == 0){

			 }

		 }
		 if(BotaoLe1 == 0){

			 if (alunosFora < 3 && alunosDentro > 0){
				 alunosFora ++;
				 alunosDentro --;
				 totalSaidas++;
				 barra(alunosDentro, alunosMax);

				 sprintf(AlunosDentro, "%-2d", alunosDentro);
				 sprintf(AlunosFora, "%-2d", alunosFora);

				 ST7735_WriteString(107,10,AlunosDentro, Font_7x10, WHITE, BLACK);
				 ST7735_WriteString(86,20, AlunosFora, Font_7x10, WHITE, BLACK);

				 HAL_Delay(200);
				 while(BotaoLe1 == 0){
				 }

			 }
		 }
		 if(BotaoLe2 == 0){
			 if (alunosFora > 0){
				 alunosDentro ++;
				 alunosFora --;
				 totalRetornos++;
				 barra(alunosDentro, alunosMax);

				 sprintf(AlunosDentro, "%-2d", alunosDentro);
				 sprintf(AlunosFora, "%-2d", alunosFora);

				 ST7735_WriteString(107,10,AlunosDentro, Font_7x10, WHITE, BLACK);
				 ST7735_WriteString(86,20, AlunosFora, Font_7x10, WHITE, BLACK);

				 HAL_Delay(200);
				 while(BotaoLe2 == 0){
				 }
			 }

		 }
		 if (BotaoLe3 == 0){

			relatorio();
			break;



		 }


	 }
 }
 void entrada(void)
 {
	 limpar;
	 bool entrandoAlunos = true;
	 char matriculaAluno[5];
	 ST7735_WriteString(0,0,"Aperte o botao 9 para entrar ", Font_7x10, WHITE, BLACK);

	 while(entrandoAlunos){
		 if(BotaoLe == 0){

			 if ( alunos < alunosMax){
				 alunos++;
				 totalEntradas++;
				 sprintf(matriculaAluno, "%d", matricula);
				 barra(alunos, alunosMax);
				 ST7735_WriteString(0,20,"Bem Vindo, Entrou: ", Font_7x10, WHITE, BLACK);
				 ST7735_WriteString(127,20,matriculaAluno, Font_7x10, WHITE, BLACK);
				 matricula++;
			 }
			 else {
		     ST7735_WriteString(0,30,"SalaCheia", Font_16x26, RED, BLACK);
			 }
			 HAL_Delay(200);
			 while(BotaoLe == 0){

			 }

		 }
		 if(BotaoLe1 == 0){
			 entrandoAlunos = false;
			 HAL_Delay(200);
			 while(BotaoLe1 == 0){

			 }
		 }

	 }
	 limpar;
	 ST7735_WriteString(0,0,"Max de alunos: ", Font_7x10, WHITE, BLACK);
	 ST7735_WriteString(99,0, AlunosMax, Font_7x10, WHITE, BLACK);
	 ST7735_WriteString(0,10,"Alunos na sala: ", Font_7x10, WHITE, BLACK);
	 ST7735_WriteString(0,20,"Alunos fora: ", Font_7x10, WHITE, BLACK);
 }

 void relatorio(void){

	char texto[10];
	 limpar;

	 uint32_t duracaoMs = HAL_GetTick() - inicioAula;
	 uint32_t minutos = duracaoMs / 60000;
	 uint32_t segundos = (duracaoMs % 60000) / 1000;


		ST7735_WriteString(0,0,"RelatorioFinal", Font_11x18, BLUE, BLACK);

		sprintf(texto, "%d", totalEntradas);
		ST7735_WriteString(0,19,"Total de alunos presentes:", Font_7x10, WHITE, BLACK);
		ST7735_WriteString(50,29,texto, Font_7x10, WHITE, BLACK);

		sprintf(texto, "%d", totalSaidas);
		ST7735_WriteString(0,39,"Total de saidas:", Font_7x10, WHITE, BLACK);
		ST7735_WriteString(113,39,texto, Font_7x10, WHITE, BLACK);

		sprintf(texto, "%d", totalRetornos);
		ST7735_WriteString(0,49,"Total de retornos:", Font_7x10, WHITE, BLACK);
		ST7735_WriteString(120,49,texto, Font_7x10, WHITE, BLACK);

		sprintf(texto, "%02ld:%02ld", minutos, segundos);
		ST7735_WriteString(0,59,"Tempo da aula:", Font_7x10, WHITE, BLACK);
		ST7735_WriteString(99,59,texto, Font_7x10, WHITE, BLACK);

		HAL_Delay(120000);
 }

 void barra(int dentro, int maximo) {
     int larguraTotal = 120;
     int larguraBarra = (dentro * larguraTotal) / maximo;

     	ST7735_FillRectangle(0, 80, larguraTotal, 10, BLACK);
        ST7735_FillRectangle(0, 80, larguraBarra, 10, GREEN);

        ST7735_FillRectangle(0, 80, 128, 1, WHITE);
        ST7735_FillRectangle(0, 90, 128, 1, WHITE);
        ST7735_FillRectangle(0, 80, 1, 10, WHITE);
        ST7735_FillRectangle(127, 80, 1, 10, WHITE);
 }

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
