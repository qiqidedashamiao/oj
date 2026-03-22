#读取文件5.txt的内容，将内容格式转换后再写入到文件test5.in中，给出python代码实现，输入文件和输出文件名称通过传入参数实现
def convert_file(input_file, output_file):
    with open(input_file, 'r') as infile:
        lines = infile.readlines()
        converted_lines = []
        line0 = lines[0].strip().strip('[]')
        line0 = line0.replace("],[", "\n")
        line0 = line0.replace(",", " ")
        line1 = lines[1].strip().strip('[]')
        line1 = line1.replace(",", " ")

        with open(output_file, 'w') as outfile:
            outfile.write(line0)
            outfile.write("\n")
            outfile.write(line1)

# 使用main函数来调用转换函数，输入文件和输出文件名称可以通过命令行参数传入
if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        print("Usage: python gene.py <input_file> <output_file>")
    else:
        input_file = sys.argv[1]
        output_file = sys.argv[2]
        convert_file(input_file, output_file)
