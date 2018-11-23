import xlrd
data = xlrd.open_workbook('mymap3.xlsx')
table = data.sheets()[0]
rows = table.nrows
cols = table.ncols
with open ('data.txt', 'w') as f:
    f.write('{')
    for i in range(0,rows) :
        f.write('{')
        for j in range(0,cols) :
            f.write(str(int(table.cell_value(i,j))))
            if(j != cols-1) :
                f.write(',')
            else :
                f.write('}')
        if(i!=rows-1) :
            f.write(',')
        f.write('\n')
    f.write('}')
