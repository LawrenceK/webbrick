#
#   Class to handle email nofications for general events
#
#    Andy Harris - 16th January 2004
#  
#
#
#
import smtplib
from email.MIMEText import MIMEText

class notify:

    def __init__ (self,notifyer,notifyee,host):
        self.to = notifyee
        self.frm =  notifyer
        try:
            self.mailObj = smtplib.SMTP(host)
            self.status = True
        except:
            print "Unable to use : %s" % host
            self.status = False


    def sendMsg (self,msgblob,subject,facility=""):
        msg = MIMEText(msgblob)
        msg['Subject'] = subject
        msg['From'] = "\"" + facility + "\"" + self.frm
        msg['To'] = self.to
        self.mailObj.sendmail(self.frm, self.to, msg.as_string())

