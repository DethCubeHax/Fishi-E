class QDEC
{
  public:

  void initQDEC()
  {
    // set pullups on inputs
    pinMode(2, OUTPUT);     //ENCA
    digitalWrite(2, 1);
    pinMode(13, OUTPUT);    //ENCB
    digitalWrite(13, 1);
    pinMode(A6, OUTPUT);    // Index
    digitalWrite(A6, 1);
  
    // Setup Quadrature Encoder with index
  
    REG_PMC_PCER0 = (1<<27); // activate clock for TC0
  
    // select XC0 as clock source and set capture mode
  
    //REG_TC0_CMR0 = 5; // continous count
    // or
    REG_TC0_CMR0 = (1<<0)|(1<<2)|(1<<8)|(1<<10); // reset counter on index
  
    // activate quadrature encoder and position measure mode, no filters
  
    REG_TC0_BMR = (1<<8)|(1<<9)|(1<<12);
  
    // enable the clock (CLKEN=1) and reset the counter (SWTRG=1) 
  
    REG_TC0_CCR0 = 5;  //
  }

  long encRead()
  {
    return REG_TC0_CV0;
  }

  void encReset()
  {
    REG_TC0_CCR0 = 5;
    REG_TC0_CCR0 = 1;
  }
};
