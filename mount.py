## read code from file
code = []
with open('output.log', 'r') as code_file:
    startReading = 0

    for line in code_file.readlines():
        if ((startReading > 0 ) and (not (line.startswith('//') or line.isspace()))):
            code = [
                line.removesuffix('\n').removesuffix('\r')
            ]
        if ("3-address code" in line): 
            print("achei a linha")
            startReading = 1


### handle variables
saved_vars = []
def fetch_var(var: str) -> str:
    var = var.strip()
    if not var.startswith('t') and var not in saved_vars:
        saved_vars.append(var)
    if var in saved_vars:
        index = saved_vars.index(var)
        var = f's{index}'
    return var
### transform code, line by line
assembly = []
for line in code:
    command = 'ERROR'
    if line.startswith('read'):
        assembly.append('li a7, 5')
        assembly.append('ecall')
        _, dest = line.split('read')
        assembly.append(f'mv {fetch_var(dest)}, a0')
    elif line.startswith('log'):
        assembly.append('li a7, 1')
        _, dest = line.split('log')
        assembly.append(f'mv a0, {fetch_var(dest)}')
        assembly.append('ecall')
    elif line.startswith('jeqz'):
        _, *args = line.split()
        assembly.append(f'beqz {", ".join(args)}')
    elif '=' in line:
        var, expression = line.split('=')
        var = fetch_var(var.strip())
        expression = expression.strip()
        if expression.isnumeric():
            command = 'li'
        elif any([var in expression for var in '-*']):
            operator,*_ = [var for var in '-*' if var in expression]
            if operator == '-':
                command = 'sub'
            elif operator == '*':
                command = 'mul'
	    # move literals to variables, mul and sub only accepts registers
            for e in expression.split(operator):
                if e.strip().isnumeric():
                    assembly.append(f'li {fetch_var(e)}, {e.strip()}')
            expression = ', '.join([
                fetch_var(e)
                for e in expression.split(operator)
            ])
        else:
            command = 'mv'
            expression = fetch_var(expression)
        assembly.append(f'{command} {var}, {expression}')
    elif line.startswith('exit'):
        _, *dest = line.split('exit')
        dest = ''.join(dest[:1])
        code = int(dest) if dest else 0
        if code != 0:
            assembly.append('li a7, 93')
            assembly.append(f'li a0, {dest}')
        else:
            assembly.append('li a7, 10')
        assembly.append('ecall')
    else:
        assembly.append(line)
# DEBUG PRINT
# print(*assembly, sep='\n')
with open('result.asm', 'w') as result_file:
   result_file.writelines([f'{line}\n' for line in assembly])
