class jkException(Exception):
    """Base class for exceptions"""
    default = None
    msgfmt = None
    code = 1

    def __init__(self, message=None):
        if not message:
            message = self.default
        elif isinstance(message, Exception):
            message = "{}: {}".format(message.__class__.__name__, message)
        if self.msgfmt:
            message = self.msgfmt.format(message)
        Exception.__init__(self, message)

class RegexException(jkException):
    msgfmt = "No se ha podido filtrar utilizando el regex \"{}\" en la fuente provista"