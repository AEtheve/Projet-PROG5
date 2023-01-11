import subprocess

def test_fusion_section(fusion_section_arm, fusion_section_gcc):
    for i in fusion_section_arm:
        if i not in fusion_section_gcc:
            print(i)
            return False
    for i in fusion_section_gcc:
        if i not in fusion_section_arm:
            print(i)
            return False
    return True
            
        
liste=[]
liste_comp_arm=[]
liste_comp_gcc=[]
fusion_section_arm=subprocess.run(['arm-none-eabi-readelf','-S','Examples_fusion/fusion_result.o'], capture_output=True, text=True).stdout
fusion_section_arm=fusion_section_arm.split('\n')

for i in range(4, len(fusion_section_arm)-6):
    liste.append([fusion_section_arm[i]])

for i in range(1,len(liste)):
    liste[i] = liste[i][0][liste[i][0].index("]")+1:]
    liste[i] = liste[i].split()
    liste_comp_arm.append([liste[i][0], liste[i][1]])

for i in liste_comp_arm:
    if i[1] == 'PROGBITS' or i[1] == 'ARM_ATTRIBUTES' or i[1] == 'NOBITS' or i[1] == 'STRTAB' :
        continue
    else:
        liste_comp_arm.remove(i)
        

subprocess.run(['gcc','./tests/test_fusion_section.c', 'gestion_section.c', 'elf32.c', 'gestion_entete.c', 'fusion_section.c', 'util.c'])
fusion_section_gcc=subprocess.run(['./a.out', 'Examples_fusion/file1.o', 'Examples_fusion/file2.o'], capture_output=True, text=True).stdout
fusion_section_gcc=fusion_section_gcc.split('\n')

liste = []
for i in range(4, len(fusion_section_gcc)-6):
    liste.append([fusion_section_gcc[i]])


for i in range(1,len(liste)):
    liste[i] = liste[i][0][liste[i][0].index("]")+1:]
    liste[i] = liste[i].split()
    liste_comp_gcc.append([liste[i][0], liste[i][1]])

if test_fusion_section(liste_comp_arm, liste_comp_gcc):
    print("Test fusion section passed")
else:    
    print("Test fusion section failed")

