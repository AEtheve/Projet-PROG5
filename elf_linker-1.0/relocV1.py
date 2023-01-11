import sys
import os
def openFile(f):
    with open(f,"r") as f_out:
        a=f_out.read()
    f_out.close()
    return a.split("\n")


def getSecName(line):
    a=line[20:21+line[21:].index("'")]
    return a
    
def getNbSectReloc(f):
    ndx=0
    l_sec=[]
    l_sec_name=[]
    for line in f:
        if line.split(' ')[0]=="Relocation":
            l_sec.append(ndx)
            l_sec_name.append(getSecName(line))
        ndx+=1
    return l_sec,l_sec_name

def findAllEntries(section, index, taillemax):
    #print(f"SECTION: {section}")
    l_entries = list()
    index+=2
    entry_count=0
    entrie='a'
    while entrie != '' and entry_count < taillemax:
        entrie = section[index].split()
        l_entries.append(entrie)
        index += 1
        entry_count+=1
    return l_entries


def getStructFile(name):
    out=openFile(name)
    l_sec,l_sec_name=getNbSectReloc(out)
    d_file = dict()
    for i in range(len(l_sec)):
        ndx=out[l_sec[i]].split(" ")[7]
        l_entries=findAllEntries(out,l_sec[i],int(ndx))
        d_file[l_sec_name[i]]=l_entries
    return d_file

def errorEntry(i,name,e1,e2):
    if i==0:
        print(f"Erreur dans l'offset affiché dans la section '{name}' entre les deux sorties:\033[91m '{e1}'\033[92m '{e2}'\033[0m (sortie/attendu)")
    elif i==1:
        print(f"Erreur dans le champ info affiché dans la section '{name}' entre les deux sorties:\033[91m '{e1}'\033[92m '{e2}'\033[0m (sortie/attendu)")
    elif i==2:
        print(f"Erreur dans le type affiché dans la section '{name}' entre les deux sorties:\033[91m '{e1}'\033[92m '{e2}'\033[0m (sortie/attendu)")
    elif i==3:
        print(f"Erreur dans la sym value affichée dans la section '{name}' entre les deux sorties:\033[91m '{e1}'\033[92m '{e2}'\033[0m (sortie/attendu)")
    elif i==4:
        print(f"Erreur dans le sym name affiché dans la section '{name}' entre les deux sorties:\033[91m '{e1}'\033[92m '{e2}'\033[0m (sortie/attendu)")



def compareEntry(e1,e2,name,warningAll=False):
    flag = True
    warning_count=0
    if len(e1)!=len(e2):
        print(f"\033[93mNombre d'entrées différents sur la section {name} entre les deux sorties\033[0m")
        warning_count+=1
        flag=False
    for i in range(len(e1)):
        ent1,ent2=e1[i],e2[i]
        for i in range(len(ent1)):
            # print(ent1[i],ent2[i])
            if ent1[i]!=ent2[i]:
                if warningAll:
                    errorEntry(i,name,ent1[i],ent2[i])
                warning_count+=1
                flag=False
    return flag,warning_count

    

def compareResults(d1,d2,warningAll=False):
    flag = True
    warning_count=0
    warning_origins=0
    if (len(d1)!=len(d2)) :
        print("\033[93mNombre de sections différent entre la sortie du programme et la sortie attendue\nErreur\n\033[0m")
        flag = False
        warning_count+=1
        warning_origins+=1
    
    for key in d1:
        try:
            entry1 = d1[key]
            entry2 = d2[key]
        except:
            print(f"\033[93mLa section {key} n'est pas présente dans la deuxième section\033[0m")
            flag = False
            warning_count+=1
            warning_origins+=1
        else:
            flag2,c2=compareEntry(entry1,entry2,key,warningAll)
            if not flag2:
                warning_count+=c2
                warning_origins+=1
            flag = flag and flag2
            
    if (flag == True):
        print("\n\033[92mLes résultats sont les memes pour le programme et la sortie arm\033[0m")
    else:
        print(f"\n\033[91mLes résultats ne sont pas correctes et comportent {warning_count} différences provenant de {warning_origins} sections différentes\033[0m")


if __name__ == "__main__":
    w=False
    if '-w' in sys.argv:
        w=True
    files=[]
    i=1
    while len(files)!=2:
        if sys.argv[i]!='-w':
            files.append(sys.argv[i])
        i+=1
    d1=getStructFile(files[0])
    d2=getStructFile(files[1])
    compareResults(d1,d2,w)
