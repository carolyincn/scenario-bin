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
data = subset(data, Type %in% c("OutInterests", "InData"))

# combine stats from all faces
data.combined = summaryBy(. ~ Time + Node + Type, data=data, FUN=sum)

data.root1 = subset (data.combined, Node == "H1")
data.root2 = subset (data.combined, Node == "H3")
data.root3 = subset (data.combined, Node == "H5")

root11 = subset(data.root1, Type=="OutInterests")
root12 = subset(data.root1, Type=="InData")

root21 = subset(data.root2, Type=="OutInterests")
root22 = subset(data.root2, Type=="InData")

root31 = subset(data.root3, Type=="OutInterests")
root32 = subset(data.root3, Type=="InData")
# root1$PacketRaw.sum / root2$PacketRaw.sum
# data.leaves = subset(data.combined, Node %in% c("leaf-1", "leaf-2", "leaf-3"))
df = data.frame(Time = root11$Time, Rate = (root12$PacketRaw.sum/root11$PacketRaw.sum+root22$PacketRaw.sum/root21$PacketRaw.sum+root32$PacketRaw.sum/root31$PacketRaw.sum) / 3)
# graph rates on all nodes in Kilobits
# g.all <- ggplot(data.combined) +
#   geom_point(aes (x=Time, y=PacketRaw.sum, color=Type), size=1) +
#   ylab("Packet Number") +
#   facet_wrap(~ Node) +
#   scale_y_continuous(breaks=seq(0, 20, 2))

# print(g.all)

# graph rates on the root nodes in Packets
g.root <- ggplot(df) +
  #ggtitle("Node H3") + 
  geom_point(aes (x=Time, y=Rate), size=2) +
  geom_line(aes (x=Time, y=Rate), size=0.5) +
  ylab("Average Satisfaction Ratio") +
  scale_x_continuous(breaks=seq(0, 25, 2)) +
  scale_y_continuous(breaks=seq(0, 1, 0.1)) +
  theme_bw() + 
  theme(plot.title=element_text(size=14,hjust=0.5),axis.title=element_text(size=14),axis.text=element_text(size=14),legend.text=element_text(size=14), legend.title=element_text(size=14))

print(g.root)

pdf("2.pdf", width=8, height=4)
print(g.root)
retval <- dev.off()