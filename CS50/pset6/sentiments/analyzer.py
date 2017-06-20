import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        self.positives = set();
        self.negatives = set();
        
        with open (positives) as lines: 
            for line in lines:
                if not line.startswith(";"):
                    self.positives.add(line.strip())
        
        
        with open (negatives) as lines:
            for line in lines:
                if not line.startswith(";"):
                   self.negatives.add(line.strip())
            

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)

        pscore = 0
        nscore = 0
        for word in tokens:
            pscore += word.lower() in self.positives
            nscore += word.lower() in self.negatives
            
        totalscore = pscore - nscore
        return int(totalscore)
            
