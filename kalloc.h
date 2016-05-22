// struct for keeping track of the percent of free physical pages
struct physPagesCounts{
  uint initPagesNo;
  uint currentFreePagesNo;
};

extern struct physPagesCounts physPagesCounts;
