require(ggplot2)
require(ggpmisc)

Data <- read.table("../data/data2.txt",stringsAsFactors = FALSE)
names(Data) <- c("Implementacao","n","m","TempoTotal","TempoReal","Threads","Erro","Comparacoes")

Data<- subset(Data,Comparacoes>0)
Data<- subset(Data,Implementacao=="s"| (Implementacao=="p" & Threads=="4"))
Data$TempoTotal[Data$Threads>0]=Data$TempoTotal[Data$Threads>0]/Data$Threads[Data$Threads>0]
Data<- subset(Data,Threads==0|(Implementacao=="p" & Threads=="4"))

d1<- subset(Data,Implementacao=="p")
d2<- subset(Data,Implementacao=="s")
d1$ComAMais<- d1$Comparacoes-d2$Comparacoes

a=paste("Média:",mean(d1$ComAMais))
qplot(x=m,xlab="m",y=ComAMais,ylab="Comparações a mais",data=d1)+geom_line()+geom_hline(yintercept = mean(d1$ComAMais), color="red")+annotate("text",x=-Inf,y=-Inf,hjust=0,vjust=0,label=a)
