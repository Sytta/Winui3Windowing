using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Windows.ApplicationModel.Activation;
using Windows.System;

namespace Windowing
{
    public sealed class ProtocolActivatedEventArgsWithWaitHandle : IProtocolActivatedEventArgs
    {
        public IProtocolActivatedEventArgs _protocolActivatedEventArgs { get; private set; }

        public string _waitHandleName { get; private set; }

        public Uri Uri => _protocolActivatedEventArgs.Uri;

        public ActivationKind Kind => _protocolActivatedEventArgs.Kind;

        public ApplicationExecutionState PreviousExecutionState => _protocolActivatedEventArgs.PreviousExecutionState;

        public SplashScreen SplashScreen => _protocolActivatedEventArgs.SplashScreen;

        public ProtocolActivatedEventArgsWithWaitHandle(IProtocolActivatedEventArgs protocolActivatedEventArgs, string waitHandleName)
        {
            _protocolActivatedEventArgs = protocolActivatedEventArgs;
            _waitHandleName = waitHandleName;
        }
    }

    public sealed class ProtocolForResultsActivatedEventArgsWithWaitHandle : IProtocolForResultsActivatedEventArgs
    {
        public IProtocolForResultsActivatedEventArgs _protocolForResultsActivatedEventArgs { get; private set; }

        public string _waitHandleName { get; private set; }

        public ProtocolForResultsOperation ProtocolForResultsOperation => _protocolForResultsActivatedEventArgs.ProtocolForResultsOperation;

        public ActivationKind Kind => _protocolForResultsActivatedEventArgs.Kind;

        public ApplicationExecutionState PreviousExecutionState => _protocolForResultsActivatedEventArgs.PreviousExecutionState;

        public SplashScreen SplashScreen => _protocolForResultsActivatedEventArgs.SplashScreen;

        public ProtocolForResultsActivatedEventArgsWithWaitHandle(IProtocolForResultsActivatedEventArgs protocolActivatedEventArgs, string waitHandleName)
        {
            _protocolForResultsActivatedEventArgs = protocolActivatedEventArgs;
            _waitHandleName = waitHandleName;
        }

    }
}
