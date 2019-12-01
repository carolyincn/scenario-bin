# Copyright (c) 2012,2015  Alexander Afanasyev <alexander.afanasyev@ucla.edu>

# install.packages('ggplot2')
library(ggplot2)
# install.packages('scales')
library(scales)

# install.packages('doBy')
library(doBy)

#########################
# Rate trace processing #
#########################
data = read.table("rate-trace.txt", header=T)
data$Node = factor(data$Node)
data$FaceId <- factor(data$FaceId)
data$PacketRaw <- data$PacketRaw
data$Type = factor(data$Type)

# exlude irrelevant types
data = subset(data, Type %in% c("InInterests", "OutInterests", "InData", "InNacks"))


# combine stats from all faces
data.combined = summaryBy(. ~ Time + Node + Type, data=data, FUN=sum)
data.root = subset(data.combined, Node == "R1-1")
data.root = subset(data.root, Time < 61)

# graph rates on the root nodes in Packets
g.root <- ggplot(data.root) +
  # ggtitle("Controller C1") + 
  geom_point(aes (x=Time, y=PacketRaw.sum, color = Type, shape = Type), size=2) +
  geom_line(aes (x=Time, y=PacketRaw.sum, color = Type), size=0.5) +
  ylab("Packets Per Second") +
  # scale_colour_discrete(labels=c("CTRL/Req", "CTRL/Resp")) +
  # scale_shape_discrete(labels=c("CTRL/Req", "CTRL/Resp")) +
  scale_x_continuous(breaks=seq(0, 180, 30)) +
  # scale_y_continuous(breaks=seq(0, 1, 0.1)) +
  theme_bw() + 
  theme(legend.position = 'bottom') +
  theme(plot.title=element_text(size=12,hjust=0.5),axis.title=element_text(size=12),axis.text=element_text(size=12),legend.text=element_text(size=12), legend.title=element_blank())

pdf("路由器.pdf", width=8, height=4)
print(g.root)
retval <- dev.off()