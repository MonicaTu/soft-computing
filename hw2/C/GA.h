/*******************************************************************/
/*                                                                 */
/*     filename : GA.h                                             */
/*     author   : edison.shih/edisonx                              */
/*     compiler : Visual C++ 2008                                  */
/*     date     : 2010.02.14                                       */
/*     maintain : 2011.11.21                                       */
/*     maintain : 2012.06.15                                       */
/*                                                                 */
/*         A.L.L.      R.I.G.H.T.S.     R.E.S.E.R.V.E.             */
/*                                                                 */
/*******************************************************************/

#ifndef __GA__
#define __GA__

#define GENETIC_LENGTH     8                     // 基因長度
#define POPULATION_CNT     10                    // 母群數量
#define ITERA_CNT          100                   // 迭代次數
#define CROSSOVER_RATE     0.5                   // 交配率
#define MUTATION_RATE      0.1                   // 突變率

// =====================================================
// 定義母體結構
typedef struct tag_parent_t{
    int genes[GENETIC_LENGTH];
    double fitness;
    double dec_value_x;
    double dec_value_y;
}parent_t;

// GAPosRand(): 隨機取得突變位置
#define GAPosRand()            (rand()%GENETIC_LENGTH)
// BinaryRand(): 隨機產生/1 整數
#define BinaryRand()           (rand()%2)
// SRand(): 隨機產生~1 的亂數
#define SRand()                ((double)rand()/(double)RAND_MAX)
// =====================================================

// 進行初始化
void initialize();
// 複製, 輪盤式選擇(分配式), 決定每個母體複製到交配池的個數
void reproduction();
// 複製, 輪盤式選擇(隨機式)
void reproduction_rnd();
// 交配, 交配池中的個體進行交配, [單點交配, 雙點交配, mask]
void crossover();
// 突變, 逐一bit 慢慢確認突變
void mutation();
// 計算基因所對應之適應值
void cal_fitness(parent_t *x);
// 計算基因對應之進制值
void cal_xvalue(parent_t *x);

// =====================================================

parent_t population[POPULATION_CNT];      // 母體數量
parent_t pool[POPULATION_CNT];            // 交配池
parent_t best_gene;                       // 從以前到現在最好的基因

// =====================================================
// binary 2 dec，將染色體中的二進位(genes) ，轉為實際可用之十進位(dec_value)
void cal_xvalue(parent_t *x)
{
    int i, dec_x=0, dec_y=0;
    for(i=0; i<GENETIC_LENGTH; i++){
        if(i<(GENETIC_LENGTH/2)) {
            if(x->genes[i] ==1) dec_x = dec_x + (0x01 << (GENETIC_LENGTH/2-1)-i);
        } else {
            if (i == (GENETIC_LENGTH/2))
            if(x->genes[i] ==1) dec_y = dec_y + (0x01 << (GENETIC_LENGTH/2-1)-(i-GENETIC_LENGTH/2));
        }
    }
    x->dec_value_x = (double)dec_x;
    x->dec_value_y = (double)dec_y;

//        printf("(%5.2lf, %5.2lf) ", x->dec_value_x, x->dec_value_y);
}

// =====================================================
// 適應函式，此設為f(x) = x*x，其中x 為染色體中之十進位，即dec_value
void cal_fitness(parent_t *x)
{
    double i = x->dec_value_x;
    double j = x->dec_value_y;
    x->fitness = (i*i + j*j);
//        printf("%5.2lf\n", x->fitness);
}

// =====================================================
// 初始化,
void initialize()
{
    int i, j;
    for(i=0; i<POPULATION_CNT; i++){
            printf("\n%d: ", i);
        for(j=0; j<GENETIC_LENGTH; j++){
            // 每個母體的基因都是隨機給/1
            population[i].genes[j] = BinaryRand();
            printf("%d", population[i].genes[j]);
        }
        // 計算母體基因之進制值
        cal_xvalue(&population[i]);
        // 計算母體對應之適應值
        cal_fitness(&population[i]);

        // 更新best_gene
        if(i==0) {
            memcpy(&best_gene,
                    &population[i],
                    sizeof(parent_t));
        } else if (population[i].fitness < best_gene.fitness){
            memcpy(&best_gene,
                    &population[i],
                    sizeof(parent_t));
        }
    }
}
// =====================================================
// 複製, 輪盤式選擇(分配式)
void reproduction()
{
    int i, j, cnt, has_copy=0;
    int Slack = POPULATION_CNT; // 還剩幾個可複制
    int pos1, pos2;
    double fitness_sum=0.0;

    // 計算所有適應值總合
    for(i=0; i<POPULATION_CNT; i++) {
        fitness_sum += population[i].fitness;
    }

    // 計算每個母體應複製幾個到交配池中，並直接做複製
    for(i=0; i<POPULATION_CNT && Slack!=0; i++) {
        // 計算應複製個數, 四捨五入
        cnt = (int)(population[i].fitness/fitness_sum + 0.5);
        // 調整可複製個數
        if(cnt > Slack) cnt=Slack;
        // 開始進行複製
        for(j=0; j<cnt; ++j, ++has_copy)
            memcpy(&pool[has_copy],
                    &population[i],
                    sizeof(parent_t));
        // 調整Slack
        Slack-=cnt;
    }
    // 若還有沒複製完的
    while(has_copy < POPULATION_CNT){
        // 隨機挑二條不同染色體出來
        pos1 = rand() % POPULATION_CNT;
        do{pos2=rand()%POPULATION_CNT;}while(pos1==pos2);
        // 比較好的那條丟到交配池去
        if(population[pos1].fitness < population[pos2].fitness) i = pos1;
        else i=pos2;
        memcpy(&pool[has_copy++],&population[i],sizeof(parent_t));
    }
}

// =====================================================
// 複製, 輪盤式選擇(隨機式)
void reproduction_rnd()
{
    int i, pos;
    double fitness_sum=0.0; // 適應值總合
    double column_prob[POPULATION_CNT];// 累計機率
    double prob; // 随機機率

    // 計算所有適應值總合
    for(i=0; i<POPULATION_CNT; i++) {
        fitness_sum += population[i].fitness;
    }
    // 計算累計機率累計分配
    column_prob[0] = population[0].fitness / fitness_sum;
    for(i=1; i<POPULATION_CNT; ++i){
        column_prob[i] =
            column_prob[i-1] + population[i].fitness / fitness_sum;
    }

    // 開始隨機抽POPULATION_CNT 個染色體到交配池
    for(i=0; i<POPULATION_CNT; ++i){
        // 產生亂數
        prob=SRand();
        // 取得落於哪一區塊
        for(pos=0; pos < POPULATION_CNT-1; ++pos){
            if(prob <= column_prob[pos] )
                break;
        }
        // 至此，pos 必為 [0, POPULATION_CNT-1]


        // 複製
        memcpy(&pool[i], &population[pos], sizeof(parent_t));
    }
}

// =====================================================
// 交配
//!< attention : 這裡只考慮母體個數為偶數個,
//!< 若母體個數為奇數個需再做額外判斷處理,否則發生記憶體寫入錯誤。

void crossover()
{
    int i, cnt=0;
    int pos=0;
    int p1, p2;
    double crossover_if;

    while(cnt < POPULATION_CNT) { //!< correct 2012.06.15
        // 隨機選二個個體
        p1 = rand() % POPULATION_CNT;
        do{p2=rand()% POPULATION_CNT;}while(p2==p1);

        crossover_if = SRand();// 決定是否交配
        if(crossover_if > CROSSOVER_RATE) {
            // 不交配, 將交配池中之個體丟回母體
            memcpy( (void *)&population[cnt++],
                    (void *)&pool[p1],
                    sizeof(parent_t));
            memcpy( (void *)&population[cnt++],
                    (void *)&pool[p2],
                    sizeof(parent_t));
        }
        else {
            // 單點交配, 交配完後再丟回母體
            do{ pos = GAPosRand();} while(pos==0);
            // crossover
            for(i=0; i<pos; i++){
                population[cnt].genes[i] = pool[p1].genes[i];
                population[cnt+1].genes[i] = pool[p2].genes[i];
            }
            for(i=pos; i<GENETIC_LENGTH; i++) {
                population[cnt+1].genes[i] = pool[p1].genes[i];
                population[cnt].genes[i] = pool[p2].genes[i];
            }
            cnt+=2; // 已複制完二條
        }
    }
}
// =====================================================
// 突變
void mutation()
{
    int i;
    int pos;
    for(i=0; i<POPULATION_CNT; i++){
        double mutation_if = SRand();
        if(mutation_if <= MUTATION_RATE){
            pos = GAPosRand();      // 突變位置
            population[i].genes[pos] =
                1 - population[i].genes[pos];
        }
        // 突變完後再算一次母體適應值
        cal_xvalue(&population[i]);  // 先算基因對應之進制x 值
        cal_fitness(&population[i]); // 再將進制x 值代入適應函式
        // 再更新best_gene
        if (population[i].fitness < best_gene.fitness){
            memcpy(&best_gene,
                    &population[i],
                    sizeof(parent_t));
        }
    }
}
// =====================================================
#endif
