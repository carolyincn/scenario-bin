# Copyright (c) 2012,2015  Alexander Afanasyev <alexander.afanasyev@ucla.edu>

# install.packages('ggplot2')
library(ggplot2)
# install.packages('scales')
library(scales)

# install.packages('doBy')
library(doBy)
library(reshape2)


#########################
# Rate trace processing #
#########################
data = read.table("比较.txt", header=T)
# data$id = seq_len(nrow(data))
# df <- melt(data, id.var = c("id", "Time"))
# names(df) <- c('id', 'Time', 'Type', 'Rate')

pdf("比较.pdf", width=8, height=4)
g.root <- ggplot(data) +
  geom_point(aes (x=Time, y=Rate, color=Type, shape = Type, group=Type), size=2) +
  geom_line(aes (x=Time, y=Rate, color=Type, group=Type), size=0.5) +
  # scale_x_continuous(breaks=seq(0, 180, 30)) +
  ylab("ASR") +
  xlab("Attackers / Legitimate hosts") +
  scale_colour_discrete(labels=c("Controller-based", "Satisfaction-based pushback")) +
  scale_shape_discrete(labels=c("Controller-based", "Satisfaction-based pushback")) +
  theme_bw() + 
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank()) +
  theme(legend.position = 'bottom') 

print(g.root)

retval <- dev.off()