#include "../globals/globals.h"
#include "../include/indicator_box_function.h"
#include "stdio.h"
#include "stdlib.h"

static struct indicator_box_square_function* data;

static int check_g(real_t* x,const real_t g_x_solution){
    real_t initial_value_x[2];initial_value_x[0]=x[0];initial_value_x[1]=x[1]; /* save the inital value, used with printf */
    real_t g_x=proxg_indicator_box_square_function(data,x);

    if(g_x_solution!=g_x){
        printf("Error Testing g(x), g([%f;%f])=%f -> theoretical g(x)=%f \n",initial_value_x[0],initial_value_x[1],g_x,g_x_solution);
        return FAILURE;
    }
    else return SUCCESS;
}

static int check_proxg(real_t* x,const real_t* proxg_x_solution){
    unsigned int i;
    real_t initial_value_x[2];initial_value_x[0]=x[0];initial_value_x[1]=x[1]; /* save the inital value, used with printf */
    proxg_indicator_box_square_function(data,x);
    unsigned char solution_is_ok=TRUE;
    for (i = 0; i < data->dimension; i++){
        if(ABS(x[i]-proxg_x_solution[i])>(1e0)*MACHINE_ACCURACY){
            solution_is_ok=FALSE;
        }
    }
    if(solution_is_ok==FALSE) {
        printf("Error Testing proxg(x), proxg([%f,%f])=[%f;%f] instead of solution=[%f,%f] \n",\
            initial_value_x[0],initial_value_x[1],x[0],x[1],proxg_x_solution[0],proxg_x_solution[1]);
        return FAILURE;
    }
    return SUCCESS;
}

int main(){
    real_t upper_bound=1;
    real_t lower_bound=-1;
    unsigned int dimension = 2;
    data =  constraint_functions_init_box(dimension,lower_bound,upper_bound,LARGE);

    data->dimension=2;
    
    /* test function g(x) */
    real_t x[2]={0,0};
    if(check_g(x,0.)==FAILURE) return FAILURE;

    x[0]=2;x[1]=0;
    if(check_g(x,LARGE)==FAILURE) return FAILURE;

    x[0]=0;x[1]=2;
    if(check_g(x,LARGE)==FAILURE) return FAILURE;

    x[0]=-2;x[1]=0;
    if(check_g(x,LARGE)==FAILURE) return FAILURE;

    x[0]=0;x[1]=-2;
    if(check_g(x,LARGE)==FAILURE) return FAILURE;

    /* test function proxg(x) */
    real_t x_solution[2]={0,0};

    x[0]=0;x[1]=0;
    x_solution[0]=0;x_solution[1]=0;
    if(check_proxg(x,x_solution)==FAILURE) return FAILURE;

    x[0]=2;x[1]=0;
    x_solution[0]=1;x_solution[1]=0;
    if(check_proxg(x,x_solution)==FAILURE) return FAILURE;

    x[0]=0;x[1]=2;
    x_solution[0]=0;x_solution[1]=1;
    if(check_proxg(x,x_solution)==FAILURE) return FAILURE;

    x[0]=-2;x[1]=0;
    x_solution[0]=-1;x_solution[1]=0;
    if(check_proxg(x,x_solution)==FAILURE) return FAILURE;

    x[0]=0;x[1]=-2;
    x_solution[0]=0;x_solution[1]=-1;
    if(check_proxg(x,x_solution)==FAILURE) return FAILURE;

    x[0]=-2;x[1]=-2;
    x_solution[0]=-1;x_solution[1]=-1;
    if(check_proxg(x,x_solution)==FAILURE) return FAILURE;

    free(data);

    return SUCCESS;
}